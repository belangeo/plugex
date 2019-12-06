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

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

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
Plugex_31_fftFilterAudioProcessor::Plugex_31_fftFilterAudioProcessor()
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

    ValueTree filterNode(Identifier("filterSavedPoints"));
    for (int i = 0; i < filterNumberOfPoints; i++) {
        filterNode.setProperty(Identifier(String(i)), 0.0f, nullptr);
    }
    parameters.state.addChild(filterNode, -1, nullptr);

    for (auto channel = 0; channel < 2; channel++) {
        fftEngine[channel].setup(lastOrder, 1 << lastOverlaps, lastWintype);
        fftEngine[channel].addListener(this);
    }

    zeromem (fftFilter, sizeof (fftFilter));

    fftFilterPoints.resize(filterNumberOfPoints);
    fftFilterPoints.fill(0.0f);
}

Plugex_31_fftFilterAudioProcessor::~Plugex_31_fftFilterAudioProcessor()
{
}

//==============================================================================
const String Plugex_31_fftFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_31_fftFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_31_fftFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_31_fftFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_31_fftFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_31_fftFilterAudioProcessor::getNumPrograms()
{
return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_31_fftFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_31_fftFilterAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_31_fftFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_31_fftFilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_31_fftFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Plugex_31_fftFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_31_fftFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_31_fftFilterAudioProcessor::computeFFTFilter() {
    int filterSize = fftEngine[0].getSize() / 2 + 1;
    for (int i = 0; i < filterSize; i++) {
        float index = sinf(i / (float)filterSize * M_PI / 2.0f) * filterNumberOfPoints;
        int ipart = (int)index;
        float fpart = index - ipart;
        fftFilter[i] = fftFilterPoints[ipart] + (fftFilterPoints[ipart+1] - fftFilterPoints[ipart]) * fpart;
    }
}

void Plugex_31_fftFilterAudioProcessor::setFFTFilterPoints(const Array<float> &value) {
    for (int i = 0; i < filterNumberOfPoints; i++) {
        fftFilterPoints.set(i, value[i]);
    }

    ValueTree filterNode = parameters.state.getChildWithName(Identifier("filterSavedPoints"));
    for (int i = 0; i < filterNumberOfPoints; i++) {
        filterNode.setProperty(Identifier(String(i)), fftFilterPoints[i], nullptr);
    }

    computeFFTFilter();
}

void Plugex_31_fftFilterAudioProcessor::fftEngineFrameReady(FFTEngine *engine, float *fftData, int fftSize) {
    for (int j = 0; j < fftSize / 2 + 1; j++) {
        float gain = fftFilter[j];
        fftData[j*2] *= gain;
        fftData[j*2+1] *= gain;
    }
}

void Plugex_31_fftFilterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    int order = (int) *orderParameter;
    int overlaps = 1 << (int) *overlapsParameter;
    int wintype = (int) *wintypeParameter;

    for (auto channel = 0; channel < totalNumInputChannels; channel++) {
        if (order != lastOrder || overlaps != lastOverlaps) {
            fftEngine[channel].setup(order, overlaps, wintype);
            computeFFTFilter();
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
bool Plugex_31_fftFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_31_fftFilterAudioProcessor::createEditor()
{
    return new Plugex_31_fftFilterAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_31_fftFilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void Plugex_31_fftFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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

    ValueTree filterNode = parameters.state.getChildWithName(Identifier("filterSavedPoints"));
    if (filterNode.isValid()) {
        for (int i = 0; i < filterNumberOfPoints; i++) {
            fftFilterPoints.set(i, (float) filterNode.getProperty(Identifier(String(i)), 0.0f));
        }
        computeFFTFilter();
        fftFilterPointsChanged = true;
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_31_fftFilterAudioProcessor();
}
