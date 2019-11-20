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
                                                     0.05f, freqSliderValueToText, freqSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("depth"), String("Depth"), String(),
                                                     NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
                                                     25.0f, depthSliderValueToText, depthSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_26_dopplerAudioProcessor::Plugex_26_dopplerAudioProcessor()
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
    depthParameter = parameters.getRawParameterValue("depth");
}

Plugex_26_dopplerAudioProcessor::~Plugex_26_dopplerAudioProcessor()
{
}

//==============================================================================
const String Plugex_26_dopplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_26_dopplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_26_dopplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_26_dopplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_26_dopplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_26_dopplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_26_dopplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_26_dopplerAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_26_dopplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_26_dopplerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_26_dopplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    lfoPhase = 0.0f;

    freqSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    freqSmoothed.setCurrentAndTargetValue(*freqParameter);
    depthSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    depthSmoothed.setCurrentAndTargetValue(*depthParameter);

    delayLine.setup(sampleRate / 2.f, currentSampleRate);
    lowpassFilter.setup(sampleRate);
    lowpassFilter.setType(0);
    lowpassFilter.setQ(0.707);
}

void Plugex_26_dopplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_26_dopplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_26_dopplerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    freqSmoothed.setTargetValue(*freqParameter);
    depthSmoothed.setTargetValue(*depthParameter);

    if (totalNumOutputChannels < 2) {
        return;
    }

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float currentFreq = freqSmoothed.getNextValue();
        float currentDepth = depthSmoothed.getNextValue() * 0.0099f;

        float sumValue = 0.0f;
        if (totalNumInputChannels == 1) {
            sumValue = buffer.getWritePointer(0)[i];
        } else {
            for (int channel = 0; channel < totalNumInputChannels; ++channel) {
                auto* channelData = buffer.getWritePointer (channel);
                sumValue += channelData[i];
            }
        }

        float lfo = jmin(lfoPhase, 1.f - lfoPhase) * 2.f;
        float delayTime = (0.5f - jmin(lfo, 1.f - lfo));
        float filterFreq = powf(jmin(lfo, 1.f - lfo), 2.f) * 20000.f + 200.f;

        auto* channelDataL = buffer.getWritePointer(0);
        auto* channelDataR = buffer.getWritePointer(1);

        lfoPhase += currentFreq / currentSampleRate;
        if (lfoPhase >= 1.0f) {
            lfoPhase -= 1.0f;
        }

        float sampleRead = delayLine.read(delayTime * currentDepth + 0.001f);
        delayLine.write(sumValue);
        lowpassFilter.setFreq(filterFreq);
        float filtered = lowpassFilter.process(sampleRead);
        channelDataL[i] = filtered * cosf(lfo * M_PI * 0.5f);
        channelDataR[i] = filtered * sinf(lfo * M_PI * 0.5f);
    }
}

//==============================================================================
bool Plugex_26_dopplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_26_dopplerAudioProcessor::createEditor()
{
    return new Plugex_26_dopplerAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_26_dopplerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_26_dopplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_26_dopplerAudioProcessor();
}
