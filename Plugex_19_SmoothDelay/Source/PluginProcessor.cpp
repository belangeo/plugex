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

static String timeSliderValueToText(float value) {
    return String(value, 2) + String(" ms");
}

static float timeSliderTextToValue(const String& text) {
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

    parameters.push_back(std::make_unique<Parameter>(String("time"), String("Time"), String(),
                                                     NormalisableRange<float>(0.01f, 1000.0f, 0.01f, 0.3f),
                                                     100.0f, timeSliderValueToText, timeSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("feedback"), String("Feedback"), String(),
                                                     NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
                                                     0.0f, feedbackSliderValueToText, feedbackSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("balance"), String("Balance"), String(),
                                                     NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
                                                     50.0f, feedbackSliderValueToText, feedbackSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_19_smoothDelayAudioProcessor::Plugex_19_smoothDelayAudioProcessor()
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
    timeParameter = parameters.getRawParameterValue("time");
    feedbackParameter = parameters.getRawParameterValue("feedback");
    balanceParameter = parameters.getRawParameterValue("balance");
}

Plugex_19_smoothDelayAudioProcessor::~Plugex_19_smoothDelayAudioProcessor()
{
}

//==============================================================================
const String Plugex_19_smoothDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_19_smoothDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_19_smoothDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_19_smoothDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_19_smoothDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_19_smoothDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_19_smoothDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_19_smoothDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_19_smoothDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_19_smoothDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_19_smoothDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    writePosition = 0;
    crossFadeTime = samplesPerBlock;
    waitingTime = 0;
    waitingClock = 0;
    currentDelay = 1;
    delay1Gain = 0.0f;
    delay2Gain = 1.0f;
    gain1Increment = gain2Increment = 0.0f;

    timeSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    timeSmoothed.setCurrentAndTargetValue(*timeParameter);
    feedbackSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    feedbackSmoothed.setCurrentAndTargetValue(*feedbackParameter);
    balanceSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    balanceSmoothed.setCurrentAndTargetValue(*balanceParameter);

    for (int channel = 0; channel < 2; channel++) {
        delayLine[channel].setup(1.f, currentSampleRate);
    }
}

void Plugex_19_smoothDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_19_smoothDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_19_smoothDelayAudioProcessor::computeDelayTime(float delayTime) {
    if (waitingClock == 0) {
        currentDelay = (currentDelay + 1) % 2;
        waitingTime = floor(delayTime * currentSampleRate + 0.5);
        if (crossFadeTime > waitingTime) {
            crossFadeTime = waitingTime;
        }
        if (crossFadeTime <= 0) {
            crossFadeTime = 1;
        }
        if (currentDelay == 0) {
            delay1Time = delayTime;
            gain1Increment = 1.0 / crossFadeTime;
            gain2Increment = -gain1Increment;
        } else {
            delay2Time = delayTime;
            gain2Increment = 1.0 / crossFadeTime;
            gain1Increment = -gain2Increment;
        }
    }
    waitingClock += 1;
    if (waitingClock == waitingTime) {
        waitingClock = 0;
    }
}

void Plugex_19_smoothDelayAudioProcessor::updateAmplitude() {
    delay1Gain += gain1Increment;
    delay1Gain = delay1Gain < 0.0f ? 0.0f : delay1Gain > 1.0f ? 1.0f : delay1Gain;
    delay2Gain += gain2Increment;
    delay2Gain = delay2Gain < 0.0f ? 0.0f : delay2Gain > 1.0f ? 1.0f : delay2Gain;
}

void Plugex_19_smoothDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    timeSmoothed.setTargetValue(*timeParameter);
    feedbackSmoothed.setTargetValue(*feedbackParameter);
    balanceSmoothed.setTargetValue(*balanceParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float currentTime = timeSmoothed.getNextValue() * 0.001;
        float currentFeedback = feedbackSmoothed.getNextValue() * 0.01;
        float currentBalance = balanceSmoothed.getNextValue() * 0.01;

        computeDelayTime(currentTime);

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float sampleRead = delayLine[channel].read(delay1Time) * delay1Gain;
            sampleRead += delayLine[channel].read(delay2Time) * delay2Gain;
            auto* channelData = buffer.getWritePointer (channel);
            delayLine[channel].write(channelData[i] + sampleRead * currentFeedback);
            channelData[i] = channelData[i] + (sampleRead - channelData[i]) * currentBalance;
        }

        updateAmplitude();
    }
}

//==============================================================================
bool Plugex_19_smoothDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_19_smoothDelayAudioProcessor::createEditor()
{
    return new Plugex_19_smoothDelayAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_19_smoothDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_19_smoothDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_19_smoothDelayAudioProcessor();
}
