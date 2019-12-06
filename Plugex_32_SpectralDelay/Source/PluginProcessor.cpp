/*******************************************************************************
* Plugex - PLUGin EXamples
*
* Plugex est une série de plugiciels auto-documentés permettant une étude 
* autonome du développement de plugiciels avec JUCE ainsi que des bases du
* traitement de signal audio avec le langage C++.
*
* © Olivier Bélanger 2019
*
*******************************************************************************/

#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("order"), String("Order"), String(),
                                                     NormalisableRange<float>(6.0f, 14.0f, 1.f, 1.0f),
                                                     10.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("overlaps"), String("Overlaps"), String(),
                                                     NormalisableRange<float>(1.0f, 3.0f, 1.f, 1.0f),
                                                     2.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("wintype"), String("Wintype"), String(),
                                                     NormalisableRange<float>(1.0f, 9.0f, 1.f, 1.0f),
                                                     3.0f, nullptr, nullptr));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_32_spectralDelayAudioProcessor::Plugex_32_spectralDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    parameters (*this, nullptr, Identifier(JucePlugin_Name), createParameterLayout())
{
    orderParameter = parameters.getRawParameterValue("order");
    overlapsParameter = parameters.getRawParameterValue("overlaps");
    wintypeParameter = parameters.getRawParameterValue("wintype");

    lastOrder = (int)*orderParameter;
    lastOverlaps = 1 << (int)*overlapsParameter;
    lastWintype = (int)*overlapsParameter;

    ValueTree delayNode(Identifier("delaySavedPoints"));
    for (int i = 0; i < multiSliderNumberOfPoints; i++) {
        delayNode.setProperty(Identifier(String(i)), 0.0f, nullptr);
    }
    parameters.state.addChild(delayNode, -1, nullptr);

    ValueTree feedbackNode(Identifier("feedbackSavedPoints"));
    for (int i = 0; i < multiSliderNumberOfPoints; i++) {
        feedbackNode.setProperty(Identifier(String(i)), 0.0f, nullptr);
    }
    parameters.state.addChild(feedbackNode, -1, nullptr);

    for (auto channel = 0; channel < 2; channel++) {
        fftEngine[channel].setup(lastOrder, 1 << lastOverlaps, lastWintype);
        fftEngine[channel].addListener(this);
    }

    zeromem (fftDelay, sizeof (fftDelay));
    zeromem (fftFeedback, sizeof (fftFeedback));

    delayPoints.resize(multiSliderNumberOfPoints);
    delayPoints.fill(0.0f);

    feedbackPoints.resize(multiSliderNumberOfPoints);
    feedbackPoints.fill(0.0f);
}

Plugex_32_spectralDelayAudioProcessor::~Plugex_32_spectralDelayAudioProcessor()
{
}

//==============================================================================
const String Plugex_32_spectralDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_32_spectralDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_32_spectralDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_32_spectralDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_32_spectralDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_32_spectralDelayAudioProcessor::getNumPrograms()
{
return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_32_spectralDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_32_spectralDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_32_spectralDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_32_spectralDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_32_spectralDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    currentSampleRate = sampleRate;
    resizeBuffers(lastOrder, lastOverlaps);
}

void Plugex_32_spectralDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_32_spectralDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Plugex_32_spectralDelayAudioProcessor::resizeBuffers(int order, int overlaps) {
    int fftSize = 1 << order;
    int hopsize = fftSize / overlaps;
    currentNumberOfFrames = (int)(maxDelayTimeInSeconds * currentSampleRate / hopsize + 0.5f);
    sampleBuffers[0].resize(currentNumberOfFrames * fftSize);
    frameCount[0] = frameCount[1] = 0;
}

void Plugex_32_spectralDelayAudioProcessor::computeFFTDelay() {
    int filterSize = fftEngine[0].getSize() / 2 + 1;
    for (int i = 0; i < filterSize; i++) {
        float index = sinf(i / (float)filterSize * M_PI / 2.0f) * multiSliderNumberOfPoints;
        int ipart = (int)index;
        float fpart = index - ipart;
        fftDelay[i] = delayPoints[ipart] + (delayPoints[ipart+1] - delayPoints[ipart]) * fpart;
    }
}

void Plugex_32_spectralDelayAudioProcessor::computeFFTFeedback() {
    int filterSize = fftEngine[0].getSize() / 2 + 1;
    for (int i = 0; i < filterSize; i++) {
        float index = sinf(i / (float)filterSize * M_PI / 2.0f) * multiSliderNumberOfPoints;
        int ipart = (int)index;
        float fpart = index - ipart;
        fftFeedback[i] = feedbackPoints[ipart] + (feedbackPoints[ipart+1] - feedbackPoints[ipart]) * fpart;
    }
}

void Plugex_32_spectralDelayAudioProcessor::setDelayPoints(const Array<float> &value) {
    ValueTree delayNode = parameters.state.getChildWithName(Identifier("delaySavedPoints"));
    for (int i = 0; i < multiSliderNumberOfPoints; i++) {
        delayPoints.set(i, value[i]);
        delayNode.setProperty(Identifier(String(i)), delayPoints[i], nullptr);
    }
    computeFFTDelay();
}

void Plugex_32_spectralDelayAudioProcessor::setFeedbackPoints(const Array<float> &value) {
    ValueTree feedbackNode = parameters.state.getChildWithName(Identifier("feedbackSavedPoints"));
    for (int i = 0; i < multiSliderNumberOfPoints; i++) {
        feedbackPoints.set(i, value[i]);
        feedbackNode.setProperty(Identifier(String(i)), feedbackPoints[i], nullptr);
    }
    computeFFTFeedback();
}

void Plugex_32_spectralDelayAudioProcessor::fftEngineFrameReady(FFTEngine *engine, float *fftData, int fftSize) {
    int channel = (engine == &fftEngine[0]) ? 0 : 1;
    int count = frameCount[channel];    

    for (int j = 0; j < fftSize / 2 + 1; j++) {
        int binDelay = (int)(fftDelay[j] * currentNumberOfFrames);
        float binFeedback = fftFeedback[j];
        binDelay = count - binDelay;
        if (binDelay < 0) {
            binDelay += currentNumberOfFrames;
        }
        if (binDelay < currentNumberOfFrames) {
            int realIndex = j * 2;
            int imagIndex = j * 2 + 1;
            float realTemp = sampleBuffers[channel][binDelay * fftSize + realIndex];
            float imagTemp = sampleBuffers[channel][binDelay * fftSize + imagIndex];
            sampleBuffers[channel].set(count * fftSize + realIndex, fftData[realIndex] + realTemp * binFeedback);
            sampleBuffers[channel].set(count * fftSize + imagIndex, fftData[imagIndex] + imagTemp * binFeedback);
            fftData[realIndex] = realTemp;
            fftData[imagIndex] = imagTemp;
        }
    }
    frameCount[channel]++;
    if (frameCount[channel] >= currentNumberOfFrames)
        frameCount[channel] = 0;
}

void Plugex_32_spectralDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    int order = (int) *orderParameter;
    int overlaps = 1 << (int) *overlapsParameter;
    int wintype = (int) *wintypeParameter;

    if (order != lastOrder || overlaps != lastOverlaps) {
        resizeBuffers(order, overlaps);
        computeFFTDelay();
        computeFFTFeedback();
    }
    for (auto channel = 0; channel < totalNumInputChannels; channel++) {
        if (order != lastOrder || overlaps != lastOverlaps) {
            fftEngine[channel].setup(order, overlaps, wintype);
        } else if (wintype != lastWintype) {
            fftEngine[channel].setWintype(wintype);
        }
        auto *channelData = buffer.getWritePointer(channel);
        for (int i = 0; i < buffer.getNumSamples(); i++) {
            channelData[i] = fftEngine[channel].process(channelData[i]);
        }
    }
    lastOrder = order;
    lastOverlaps = overlaps;
    lastWintype = wintype;
}

//==============================================================================
bool Plugex_32_spectralDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_32_spectralDelayAudioProcessor::createEditor()
{
    return new Plugex_32_spectralDelayAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_32_spectralDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void Plugex_32_spectralDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr) {
        if (xmlState->hasTagName (parameters.state.getType())) {
            ValueTree state = ValueTree::fromXml (*xmlState);
            parameters.replaceState (state);
        }
    }

    ValueTree delayNode = parameters.state.getChildWithName(Identifier("delaySavedPoints"));
    if (delayNode.isValid()) {
        for (int i = 0; i < multiSliderNumberOfPoints; i++) {
            delayPoints.set(i, (float) delayNode.getProperty(Identifier(String(i)), 0.0f));
        }
        computeFFTDelay();
        delayPointsChanged = true;
    }

    ValueTree feedbackNode = parameters.state.getChildWithName(Identifier("feedbackSavedPoints"));
    if (feedbackNode.isValid()) {
        for (int i = 0; i < multiSliderNumberOfPoints; i++) {
            feedbackPoints.set(i, (float) feedbackNode.getProperty(Identifier(String(i)), 0.0f));
        }
        computeFFTFeedback();
        feedbackPointsChanged = true;
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_32_spectralDelayAudioProcessor();
}
