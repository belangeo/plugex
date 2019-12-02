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

#include <cmath>
#include "PluginProcessor.h"
#include "PluginEditor.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

static String responseSliderValueToText(float value) {
    return String(value, 2) + String(" Hz");
}

static float responseSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String maxFreqSliderValueToText(float value) {
    return String(value, 2) + String(" Hz");
}

static float maxFreqSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String qSliderValueToText(float value) {
    return String(value, 2) + String(" Q");
}

static float qSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("response"), String("Response"), String(),
                                                     NormalisableRange<float>(0.1f, 100.0f, 0.01f, 0.3f),
                                                     10.0f, responseSliderValueToText, responseSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("maxFreq"), String("MaxFreq"), String(),
                                                     NormalisableRange<float>(500.0f, 10000.0f, 0.01f, 0.3f),
                                                     5000.0f, maxFreqSliderValueToText, maxFreqSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("q"), String("Q"), String(),
                                                     NormalisableRange<float>(0.7f, 20.0f, 0.01f, 0.5f),
                                                     5.0f, qSliderValueToText, qSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_27_autoWahAudioProcessor::Plugex_27_autoWahAudioProcessor()
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
    responseParameter = parameters.getRawParameterValue("response");
    maxFreqParameter = parameters.getRawParameterValue("maxFreq");
    qParameter = parameters.getRawParameterValue("q");
}

Plugex_27_autoWahAudioProcessor::~Plugex_27_autoWahAudioProcessor()
{
}

//==============================================================================
const String Plugex_27_autoWahAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_27_autoWahAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_27_autoWahAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_27_autoWahAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_27_autoWahAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_27_autoWahAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_27_autoWahAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_27_autoWahAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_27_autoWahAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_27_autoWahAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_27_autoWahAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    responseSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    responseSmoothed.setCurrentAndTargetValue(*responseParameter);
    maxFreqSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    maxFreqSmoothed.setCurrentAndTargetValue(*maxFreqParameter);
    qSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    qSmoothed.setCurrentAndTargetValue(*qParameter);

    for (int channel = 0; channel < 2; channel++) {
        lowpassFilter[channel].setup(currentSampleRate);
        bandpassFilter[channel].setup(currentSampleRate);
        bandpassFilter[channel].setType(2);
    }
}

void Plugex_27_autoWahAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_27_autoWahAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_27_autoWahAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    responseSmoothed.setTargetValue(*responseParameter);
    maxFreqSmoothed.setTargetValue(*maxFreqParameter);
    qSmoothed.setTargetValue(*qParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float currentResponse = responseSmoothed.getNextValue();
        float currentMaxFreq = maxFreqSmoothed.getNextValue();
        float currentQ = qSmoothed.getNextValue();

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            lowpassFilter[channel].setFreq(currentResponse);
            auto* channelData = buffer.getWritePointer (channel);
            float rectified = channelData[i] < 0.0f ? -channelData[i] : channelData[i];
            float follower = lowpassFilter[channel].process(rectified);
            bandpassFilter[channel].setFreq(follower * (currentMaxFreq - 250.0f) + 250.0f);
            bandpassFilter[channel].setQ(currentQ);
            channelData[i] = bandpassFilter[channel].process(channelData[i]);
        }
    }
}

//==============================================================================
bool Plugex_27_autoWahAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_27_autoWahAudioProcessor::createEditor()
{
    return new Plugex_27_autoWahAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_27_autoWahAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_27_autoWahAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_27_autoWahAudioProcessor();
}
