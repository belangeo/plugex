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

static String freqSliderValueToText(float value) {
    return String(value, 3) + String(" Hz");
}

static float freqSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String delaySliderValueToText(float value) {
    return String(value, 2) + String(" ms");
}

static float delaySliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String depthSliderValueToText(float value) {
    return String(value, 2) + String(" %");
}

static float depthSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("freq"), String("Freq"), String(),
                                                     NormalisableRange<float>(0.01f, 1.0f, 0.001f, 0.5f),
                                                     0.1f, freqSliderValueToText, freqSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("delay"), String("Delay"), String(),
                                                     NormalisableRange<float>(1.0f, 10.0f, 0.01f, 1.0f),
                                                     5.0f, delaySliderValueToText, delaySliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("depth"), String("Depth"), String(),
                                                     NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
                                                     100.0f, depthSliderValueToText, depthSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_20_flangerAudioProcessor::Plugex_20_flangerAudioProcessor()
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
    delayParameter = parameters.getRawParameterValue("delay");
    depthParameter = parameters.getRawParameterValue("depth");
}

Plugex_20_flangerAudioProcessor::~Plugex_20_flangerAudioProcessor()
{
}

//==============================================================================
const String Plugex_20_flangerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_20_flangerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_20_flangerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_20_flangerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_20_flangerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_20_flangerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_20_flangerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_20_flangerAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_20_flangerAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_20_flangerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_20_flangerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    lfoPhase = 0.0f;

    freqSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    freqSmoothed.setCurrentAndTargetValue(*freqParameter);
    delaySmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    delaySmoothed.setCurrentAndTargetValue(*delayParameter);
    depthSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    depthSmoothed.setCurrentAndTargetValue(*depthParameter);

    for (int channel = 0; channel < 2; channel++) {
        delayLine[channel].setup(0.25f, currentSampleRate);
    }
}

void Plugex_20_flangerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_20_flangerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_20_flangerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    freqSmoothed.setTargetValue(*freqParameter);
    delaySmoothed.setTargetValue(*delayParameter);
    depthSmoothed.setTargetValue(*depthParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float currentFreq = freqSmoothed.getNextValue();
        float currentDelay = delaySmoothed.getNextValue() * 0.001f;
        float currentDepth = depthSmoothed.getNextValue() * 0.0099f;

        float lfoDelayTime = sinf(lfoPhase * M_PI * 2) * (currentDelay * currentDepth) + currentDelay;
        lfoPhase += currentFreq / currentSampleRate;
        if (lfoPhase >= 1.0f) {
            lfoPhase -= 1.0f;
        }

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float sampleRead = delayLine[channel].read(lfoDelayTime);
            auto* channelData = buffer.getWritePointer (channel);
            delayLine[channel].write(channelData[i]);
            channelData[i] = channelData[i] + (sampleRead - channelData[i]) * 0.5f;
        }
    }
}

//==============================================================================
bool Plugex_20_flangerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_20_flangerAudioProcessor::createEditor()
{
    return new Plugex_20_flangerAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_20_flangerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_20_flangerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_20_flangerAudioProcessor();
}
