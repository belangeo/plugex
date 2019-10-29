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

static String percentSliderValueToText(float value) {
    return String(value, 2) + String(" %");
}

static float percentSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String cutoffSliderValueToText(float value) {
    return String(value, 2) + String(" Hz");
}

static float cutoffSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("percent"), String("Percent"), String(),
                                                     NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
                                                     0.0f, percentSliderValueToText, percentSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("cutoff"), String("Cutoff"), String(),
                                                     NormalisableRange<float>(20.0f, 18000.0f, 0.01f, 0.3f),
                                                     1000.0f, cutoffSliderValueToText, cutoffSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_14_rectifierAudioProcessor::Plugex_14_rectifierAudioProcessor()
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
    percentParameter = parameters.getRawParameterValue("percent");
    cutoffParameter = parameters.getRawParameterValue("cutoff");
}

Plugex_14_rectifierAudioProcessor::~Plugex_14_rectifierAudioProcessor()
{
}

//==============================================================================
const String Plugex_14_rectifierAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_14_rectifierAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_14_rectifierAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_14_rectifierAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_14_rectifierAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_14_rectifierAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_14_rectifierAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_14_rectifierAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_14_rectifierAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_14_rectifierAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_14_rectifierAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    percentSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    percentSmoothed.setCurrentAndTargetValue(*percentParameter);
    cutoffSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    cutoffSmoothed.setCurrentAndTargetValue(*cutoffParameter);

    for (int channel = 0; channel < 2; channel++) {
        lowpassFilter[channel].setup(sampleRate);
    }
}

void Plugex_14_rectifierAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_14_rectifierAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_14_rectifierAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    percentSmoothed.setTargetValue(*percentParameter);
    cutoffSmoothed.setTargetValue(*cutoffParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float currentPercent = percentSmoothed.getNextValue() * 0.01f;
        float currentCutoff = cutoffSmoothed.getNextValue();
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            lowpassFilter[channel].setFreq(currentCutoff);
            auto* channelData = buffer.getWritePointer (channel);
            channelData[i] = channelData[i] < 0.0f ? channelData[i] + (-channelData[i] - channelData[i]) * currentPercent : channelData[i];
            channelData[i] = lowpassFilter[channel].process(channelData[i]);
        }
    }
}

//==============================================================================
bool Plugex_14_rectifierAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_14_rectifierAudioProcessor::createEditor()
{
    return new Plugex_14_rectifierAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_14_rectifierAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_14_rectifierAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_14_rectifierAudioProcessor();
}
