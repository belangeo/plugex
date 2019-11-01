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

static String freqSliderValueToText(float value) {
    return String(value, 2) + String(" Hz");
}

static float freqSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String spreadSliderValueToText(float value) {
    return String(value, 2) + String(" x");
}

static float spreadSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String qSliderValueToText(float value) {
    return String(value, 2) + String(" Q");
}

static float qSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String feedbackSliderValueToText(float value) {
    return String(value, 2) + String(" %");
}

static float feedbackSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("freq"), String("Freq"), String(),
                                                     NormalisableRange<float>(20.0f, 10000.0f, 0.01f, 0.3f),
                                                     100.0f, freqSliderValueToText, freqSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("spread"), String("spread"), String(),
                                                     NormalisableRange<float>(0.5f, 2.0f, 0.01f, 1.0f),
                                                     1.25f, spreadSliderValueToText, spreadSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("q"), String("Q"), String(),
                                                     NormalisableRange<float>(0.5f, 20.0f, 0.01f, 0.3f),
                                                     1.0f, qSliderValueToText, qSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("feedback"), String("Feedback"), String(),
                                                     NormalisableRange<float>(0.f, 100.0f, 0.01f, 1.0f),
                                                     25.0f, feedbackSliderValueToText, feedbackSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_23_phaserAudioProcessor::Plugex_23_phaserAudioProcessor()
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
    freqParameter = parameters.getRawParameterValue("freq");
    spreadParameter = parameters.getRawParameterValue("spread");
    qParameter = parameters.getRawParameterValue("q");
    feedbackParameter = parameters.getRawParameterValue("feedback");
}

Plugex_23_phaserAudioProcessor::~Plugex_23_phaserAudioProcessor()
{
}

//==============================================================================
const String Plugex_23_phaserAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_23_phaserAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_23_phaserAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_23_phaserAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_23_phaserAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_23_phaserAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_23_phaserAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_23_phaserAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_23_phaserAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_23_phaserAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_23_phaserAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    freqSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    freqSmoothed.setCurrentAndTargetValue(*freqParameter);
    spreadSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    spreadSmoothed.setCurrentAndTargetValue(*spreadParameter);
    qSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    qSmoothed.setCurrentAndTargetValue(*qParameter);
    feedbackSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    feedbackSmoothed.setCurrentAndTargetValue(*feedbackParameter);

    for (int filter = 0; filter < 8; filter++) {
        for (int channel = 0; channel < 2; channel++) {
            allpassFilter[filter][channel].setup(currentSampleRate);
        }
    }
    lastFilterOutput[0] = lastFilterOutput[1] = 0.0f;
}

void Plugex_23_phaserAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_23_phaserAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_23_phaserAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    freqSmoothed.setTargetValue(*freqParameter);
    spreadSmoothed.setTargetValue(*spreadParameter);
    qSmoothed.setTargetValue(*qParameter);
    feedbackSmoothed.setTargetValue(*feedbackParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float freq = freqSmoothed.getNextValue();
        float spread = spreadSmoothed.getNextValue();
        float q = qSmoothed.getNextValue();
        float feedback = feedbackSmoothed.getNextValue() * 0.01f;

        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            auto* channelData = buffer.getWritePointer (channel);
            float filterFreq = freq;
            float filterInput = channelData[i] + lastFilterOutput[channel] * feedback;
            for (int filter = 0; filter < 8; filter++) {
                allpassFilter[filter][channel].setParameters(filterFreq, q, 4);
                filterInput = allpassFilter[filter][channel].process(filterInput);
                filterFreq *= spread;
            }
            lastFilterOutput[channel] = filterInput;
            channelData[i] = (channelData[i] + filterInput) * 0.7071f;
        }
    }
}

//==============================================================================
bool Plugex_23_phaserAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_23_phaserAudioProcessor::createEditor()
{
    return new Plugex_23_phaserAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_23_phaserAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_23_phaserAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_23_phaserAudioProcessor();
}
