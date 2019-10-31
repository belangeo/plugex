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

static String depthSliderValueToText(float value) {
    return String(value, 2) + String(" %");
}

static float depthSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("depth"), String("Depth"), String(),
                                                     NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
                                                     25.0f, depthSliderValueToText, depthSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("feedback"), String("Feedback"), String(),
                                                     NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
                                                     40.0f, depthSliderValueToText, depthSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("balance"), String("Balance"), String(),
                                                     NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
                                                     50.0f, depthSliderValueToText, depthSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_21_chorusAudioProcessor::Plugex_21_chorusAudioProcessor()
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
    depthParameter = parameters.getRawParameterValue("depth");
    feedbackParameter = parameters.getRawParameterValue("feedback");
    balanceParameter = parameters.getRawParameterValue("balance");
}

Plugex_21_chorusAudioProcessor::~Plugex_21_chorusAudioProcessor()
{
}

//==============================================================================
const String Plugex_21_chorusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_21_chorusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_21_chorusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_21_chorusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_21_chorusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_21_chorusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_21_chorusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_21_chorusAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_21_chorusAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_21_chorusAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_21_chorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    depthSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    depthSmoothed.setCurrentAndTargetValue(*depthParameter);
    feedbackSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    feedbackSmoothed.setCurrentAndTargetValue(*feedbackParameter);
    balanceSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    balanceSmoothed.setCurrentAndTargetValue(*balanceParameter);

    float srFactor = currentSampleRate / 44100.f;

    for (int line = 0; line < 8; line++) {
        for (int channel = 0; channel < 2; channel++) {
            lfoDelayTime[line][channel].setup(currentSampleRate);
            float delayLineLength = (centerDelayTimes[0] * srFactor * 2.f + 0.005f);
            delayLine[line][channel].setup(delayLineLength, currentSampleRate);
        }
    }
}

void Plugex_21_chorusAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_21_chorusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_21_chorusAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    depthSmoothed.setTargetValue(*depthParameter);
    feedbackSmoothed.setTargetValue(*feedbackParameter);
    balanceSmoothed.setTargetValue(*balanceParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        float currentDepth = depthSmoothed.getNextValue() * 0.02;
        float currentFeedback = feedbackSmoothed.getNextValue() * 0.01;
        float currentBalance = balanceSmoothed.getNextValue() * 0.01;

        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            float totalSignal = 0.f;
            auto* channelData = buffer.getWritePointer (channel);
            for (int line = 0; line < 8; line++) {
                lfoDelayTime[line][channel].setFreq(lfoFrequencies[line] + channel * 0.01f);
                float delayTime = delayTimeDevs[line] * currentDepth * lfoDelayTime[line][channel].process() + centerDelayTimes[line];
                float sampleRead = delayLine[line][channel].read(delayTime);
                delayLine[line][channel].write(channelData[i] + sampleRead * currentFeedback);
                totalSignal += sampleRead;
            }
            totalSignal *= 0.25;
            channelData[i] = channelData[i] + (totalSignal - channelData[i]) * currentBalance;
        }
    }
}

//==============================================================================
bool Plugex_21_chorusAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_21_chorusAudioProcessor::createEditor()
{
    return new Plugex_21_chorusAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_21_chorusAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_21_chorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_21_chorusAudioProcessor();
}
