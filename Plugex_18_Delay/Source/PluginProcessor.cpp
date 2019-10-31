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
Plugex_18_delayAudioProcessor::Plugex_18_delayAudioProcessor()
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

Plugex_18_delayAudioProcessor::~Plugex_18_delayAudioProcessor()
{
}

//==============================================================================
const String Plugex_18_delayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_18_delayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_18_delayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_18_delayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_18_delayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_18_delayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_18_delayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_18_delayAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_18_delayAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_18_delayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_18_delayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    writePosition = 0;

    timeSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    timeSmoothed.setCurrentAndTargetValue(*timeParameter);
    feedbackSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    feedbackSmoothed.setCurrentAndTargetValue(*feedbackParameter);
    balanceSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    balanceSmoothed.setCurrentAndTargetValue(*balanceParameter);

    for (int channel = 0; channel < 2; channel++) {
        delayLine[channel].reset( new float[static_cast<int>(currentSampleRate) + 1]);
        for (int i = 0; i < static_cast<int>(currentSampleRate) + 1; i++) {
            delayLine[channel][i] = 0.0f;
        }
    }
}

void Plugex_18_delayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_18_delayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_18_delayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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
        float currentTime = timeSmoothed.getNextValue();
        float currentFeedback = feedbackSmoothed.getNextValue() * 0.01;
        float currentBalance = balanceSmoothed.getNextValue() * 0.01;
        float delayTimeInSamples = currentTime * 0.001 * currentSampleRate;
        float readPosition = writePosition - delayTimeInSamples;
        if (readPosition < 0) {
            readPosition += currentSampleRate;
        } else if (readPosition >= currentSampleRate) {
            readPosition -= currentSampleRate;
        }
        int readPosIntegerPart = static_cast<int>(readPosition);
        float readPosFloatPart = readPosition - readPosIntegerPart;
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float previous = delayLine[channel][readPosIntegerPart];
            float next = delayLine[channel][readPosIntegerPart + 1];
            float sampleRead = previous + (next - previous) * readPosFloatPart;
            auto* channelData = buffer.getWritePointer (channel);
            delayLine[channel][writePosition] = channelData[i] + sampleRead * currentFeedback;
            if (writePosition == 0) {
                delayLine[channel][currentSampleRate] = delayLine[channel][0];
            }
            channelData[i] = channelData[i] + (sampleRead - channelData[i]) * currentBalance;
        }
        if (writePosition++ == currentSampleRate) {
            writePosition = 0;
        }
    }
}

//==============================================================================
bool Plugex_18_delayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_18_delayAudioProcessor::createEditor()
{
    return new Plugex_18_delayAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_18_delayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_18_delayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_18_delayAudioProcessor();
}
