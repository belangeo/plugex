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

static String qSliderValueToText(float value) {
    return String(value, 2) + String(" Q");
}

static float qSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String driveSliderValueToText(float value) {
    return String(value, 3) + String(" x");
}

static float driveSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String balanceSliderValueToText(float value) {
    return String(value, 2) + String(" %");
}

static float balanceSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("highpassFreq"), String("Highpass Freq"), String(),
                                                     NormalisableRange<float>(20.0f, 1000.0f, 0.01f, 0.3f),
                                                     100.0f, freqSliderValueToText, freqSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("highpassQ"), String("Highpass Q"), String(),
                                                     NormalisableRange<float>(0.5f, 20.0f, 0.01f, 1.0f),
                                                     1.0f, qSliderValueToText, qSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("drive"), String("Drive"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f),
                                                     0.75f, driveSliderValueToText, driveSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("lowpassFreq"), String("Lowpass Freq"), String(),
                                                     NormalisableRange<float>(100.0f, 15000.0f, 0.01f, 0.3f),
                                                     4000.0f, freqSliderValueToText, freqSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("lowpassQ"), String("Lowpass Q"), String(),
                                                     NormalisableRange<float>(0.5f, 20.0f, 0.01f, 1.0f),
                                                     1.0f, qSliderValueToText, qSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("balance"), String("Balance"), String(),
                                                     NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
                                                     50.0f, balanceSliderValueToText, balanceSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_17_fullDistortionAudioProcessor::Plugex_17_fullDistortionAudioProcessor()
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
    highpassFreqParameter = parameters.getRawParameterValue("highpassFreq");
    highpassQParameter = parameters.getRawParameterValue("highpassQ");
    driveParameter = parameters.getRawParameterValue("drive");
    lowpassFreqParameter = parameters.getRawParameterValue("lowpassFreq");
    lowpassQParameter = parameters.getRawParameterValue("lowpassQ");
    balanceParameter = parameters.getRawParameterValue("balance");
}

Plugex_17_fullDistortionAudioProcessor::~Plugex_17_fullDistortionAudioProcessor()
{
}

//==============================================================================
const String Plugex_17_fullDistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_17_fullDistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_17_fullDistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_17_fullDistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_17_fullDistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_17_fullDistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_17_fullDistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_17_fullDistortionAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_17_fullDistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_17_fullDistortionAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_17_fullDistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    highpassFreqSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    highpassFreqSmoothed.setCurrentAndTargetValue(*highpassFreqParameter);
    highpassQSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    highpassQSmoothed.setCurrentAndTargetValue(*highpassQParameter);
    driveSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    driveSmoothed.setCurrentAndTargetValue(*driveParameter);
    lowpassFreqSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    lowpassFreqSmoothed.setCurrentAndTargetValue(*lowpassFreqParameter);
    lowpassQSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    lowpassQSmoothed.setCurrentAndTargetValue(*lowpassQParameter);
    balanceSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    balanceSmoothed.setCurrentAndTargetValue(*balanceParameter);

    for (int channel = 0; channel < 8; channel++) {
        highpassFilter[channel].setup(sampleRate);
        lowpassFilter[channel].setup(sampleRate);
    }
}

void Plugex_17_fullDistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_17_fullDistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_17_fullDistortionAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    highpassFreqSmoothed.setTargetValue(*highpassFreqParameter);
    highpassQSmoothed.setTargetValue(*highpassQParameter);
    driveSmoothed.setTargetValue(*driveParameter);
    lowpassFreqSmoothed.setTargetValue(*lowpassFreqParameter);
    lowpassQSmoothed.setTargetValue(*lowpassQParameter);
    balanceSmoothed.setTargetValue(*balanceParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float currentHighpassFreq = highpassFreqSmoothed.getNextValue();
        float currentHighpassQ = highpassQSmoothed.getNextValue();
        float currentDrive = driveSmoothed.getNextValue() * 0.998;
        float currentLowpassFreq = lowpassFreqSmoothed.getNextValue();
        float currentLowpassQ = lowpassQSmoothed.getNextValue();
        float currentBalance = balanceSmoothed.getNextValue() * 0.01;
        float shapeFactor = (2.0f * currentDrive) / (1.0f - currentDrive);
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            highpassFilter[channel].setParameters(currentHighpassFreq, currentHighpassQ, 1);
            lowpassFilter[channel].setParameters(currentLowpassFreq, currentLowpassQ, 0);
            auto* channelData = buffer.getWritePointer (channel);
            float channelInput = channelData[i];
            channelData[i] = highpassFilter[channel].process(channelData[i]);
            channelData[i] = (1.0f + shapeFactor) * channelData[i] / (1.0f + shapeFactor * fabsf(channelData[i])) * 0.7;
            channelData[i] = lowpassFilter[channel].process(channelData[i]);
            channelData[i] = channelInput + (channelData[i] - channelInput) * currentBalance;
        }
    }
}

//==============================================================================
bool Plugex_17_fullDistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_17_fullDistortionAudioProcessor::createEditor()
{
    return new Plugex_17_fullDistortionAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_17_fullDistortionAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_17_fullDistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_17_fullDistortionAudioProcessor();
}
