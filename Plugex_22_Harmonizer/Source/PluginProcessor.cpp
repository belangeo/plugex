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

static String transpoSliderValueToText(float value) {
    return String(value, 2) + String(" semi");
}

static float transpoSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String feedbackSliderValueToText(float value) {
    return String(value, 2) + String(" %");
}

static float feedbackSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String winsizeSliderValueToText(float value) {
    return String(value, 2) + String(" ms");
}

static float winsizeSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("transpo"), String("Transpo"), String(),
                                                     NormalisableRange<float>(-24.0f, 24.0f, 0.01f, 1.0f),
                                                     -7.0f, transpoSliderValueToText, transpoSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("feedback"), String("Feedback"), String(),
                                                     NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
                                                     0.0f, feedbackSliderValueToText, feedbackSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("winsize"), String("Winsize"), String(),
                                                     NormalisableRange<float>(10.0f, 200.0f, 0.01f, 1.0f),
                                                     100.0f, winsizeSliderValueToText, winsizeSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("balance"), String("Balance"), String(),
                                                     NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
                                                     50.0f, feedbackSliderValueToText, feedbackSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_22_harmonizerAudioProcessor::Plugex_22_harmonizerAudioProcessor()
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
    transpoParameter = parameters.getRawParameterValue("transpo");
    feedbackParameter = parameters.getRawParameterValue("feedback");
    winsizeParameter = parameters.getRawParameterValue("winsize");
    balanceParameter = parameters.getRawParameterValue("balance");
}

Plugex_22_harmonizerAudioProcessor::~Plugex_22_harmonizerAudioProcessor()
{
}

//==============================================================================
const String Plugex_22_harmonizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_22_harmonizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_22_harmonizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_22_harmonizerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_22_harmonizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_22_harmonizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_22_harmonizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_22_harmonizerAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_22_harmonizerAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_22_harmonizerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_22_harmonizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    runningPhase = 0.0f;

    transpoSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    transpoSmoothed.setCurrentAndTargetValue(*transpoParameter);
    feedbackSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    feedbackSmoothed.setCurrentAndTargetValue(*feedbackParameter);
    winsizeSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    winsizeSmoothed.setCurrentAndTargetValue(*winsizeParameter);
    balanceSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    balanceSmoothed.setCurrentAndTargetValue(*balanceParameter);

    for (int channel = 0; channel < 2; channel++) {
        delayLine[channel].setup(0.25f, currentSampleRate);
        dcFilterLastInput[channel] = dcFilterLastOutput[channel] = 0.0f;
    }
}

void Plugex_22_harmonizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_22_harmonizerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_22_harmonizerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    transpoSmoothed.setTargetValue(*transpoParameter);
    feedbackSmoothed.setTargetValue(*feedbackParameter);
    winsizeSmoothed.setTargetValue(*winsizeParameter);
    balanceSmoothed.setTargetValue(*balanceParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float currentTranspo = transpoSmoothed.getNextValue();
        float currentFeedback = feedbackSmoothed.getNextValue() * 0.01;
        float currentWinsize = winsizeSmoothed.getNextValue() * 0.001;
        float currentBalance = balanceSmoothed.getNextValue() * 0.01;

        float ratio = powf(2.0f, currentTranspo / 12.0f);
        float rate = (ratio - 1.0f) / currentWinsize;
        float inc = -rate / currentSampleRate;

        float firstOverlapAmp = sinf(runningPhase * M_PI);
        float secondOverlapAmp = sinf(fmod(runningPhase + 0.5f, 1.0f) * M_PI);
        float firstOverlapDelay = runningPhase * currentWinsize;
        float secondOverlapDelay = fmod(runningPhase + 0.5f, 1.0f) * currentWinsize;
    
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float sampleRead = delayLine[channel].read(firstOverlapDelay) * firstOverlapAmp;
            sampleRead += delayLine[channel].read(secondOverlapDelay) * secondOverlapAmp;
            auto* channelData = buffer.getWritePointer (channel);
            dcFilterLastOutput[channel] = sampleRead - dcFilterLastInput[channel] + 0.995 * dcFilterLastOutput[channel];
            dcFilterLastInput[channel] = sampleRead;
            delayLine[channel].write(channelData[i] + dcFilterLastOutput[channel] * currentFeedback);
            channelData[i] = channelData[i] + (sampleRead - channelData[i]) * currentBalance;
        }

        runningPhase += inc;
        if (runningPhase < 0) {
            runningPhase += 1.0f;
        } else if (runningPhase >= 1.0f) {
            runningPhase -= 1.0f;
        }
    }
}

//==============================================================================
bool Plugex_22_harmonizerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_22_harmonizerAudioProcessor::createEditor()
{
    return new Plugex_22_harmonizerAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_22_harmonizerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_22_harmonizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_22_harmonizerAudioProcessor();
}
