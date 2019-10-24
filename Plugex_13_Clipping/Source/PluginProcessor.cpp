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

static String threshSliderValueToText(float value) {
    return String(value, 3) + String(" x");
}

static float threshSliderTextToValue(const String& text) {
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

    parameters.push_back(std::make_unique<Parameter>(String("thresh"), String("Thresh"), String(),
                                                     NormalisableRange<float>(0.01f, 1.0f, 0.001f, 0.5f),
                                                     0.5f, threshSliderValueToText, threshSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("cutoff"), String("Cutoff"), String(),
                                                     NormalisableRange<float>(20.0f, 18000.0f, 0.01f, 0.3f),
                                                     1000.0f, cutoffSliderValueToText, cutoffSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_13_clippingAudioProcessor::Plugex_13_clippingAudioProcessor()
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
    threshParameter = parameters.getRawParameterValue("thresh");
    cutoffParameter = parameters.getRawParameterValue("cutoff");
}

Plugex_13_clippingAudioProcessor::~Plugex_13_clippingAudioProcessor()
{
}

//==============================================================================
const String Plugex_13_clippingAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_13_clippingAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_13_clippingAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_13_clippingAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_13_clippingAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_13_clippingAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_13_clippingAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_13_clippingAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_13_clippingAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_13_clippingAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_13_clippingAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    threshSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    threshSmoothed.setCurrentAndTargetValue(*threshParameter);
    cutoffSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    cutoffSmoothed.setCurrentAndTargetValue(*cutoffParameter);

    for (int channel = 0; channel < 8; channel++) {
        lowpassFilter[channel].setup(sampleRate);
    }
}

void Plugex_13_clippingAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_13_clippingAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_13_clippingAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    threshSmoothed.setTargetValue(*threshParameter);
    cutoffSmoothed.setTargetValue(*cutoffParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float currentThresh = threshSmoothed.getNextValue();
        float currentCutoff = cutoffSmoothed.getNextValue();
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            lowpassFilter[channel].setFreq(currentCutoff);
            auto* channelData = buffer.getWritePointer (channel);
            channelData[i] = channelData[i] > currentThresh ? currentThresh : channelData[i] < -currentThresh ? -currentThresh : channelData[i];
            channelData[i] *= 0.7071f / currentThresh;
            channelData[i] = lowpassFilter[channel].process(channelData[i]);
        }
    }
}

//==============================================================================
bool Plugex_13_clippingAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_13_clippingAudioProcessor::createEditor()
{
    return new Plugex_13_clippingAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_13_clippingAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_13_clippingAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_13_clippingAudioProcessor();
}
