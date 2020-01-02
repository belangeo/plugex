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

static String srscaleSliderValueToText(float value) {
    return String(value, 2) + String(" x");
}

static float srscaleSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String bitdepthSliderValueToText(float value) {
    return String(value, 2) + String(" bit");
}

static float bitdepthSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("srscale"), String("SR Scale"), String(),
                                                     NormalisableRange<float>(0.01f, 1.0f, 0.01f, 0.5f),
                                                     1.0f, srscaleSliderValueToText, srscaleSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("bitdepth"), String("Bit Depth"), String(),
                                                     NormalisableRange<float>(2.0f, 16.0f, 0.01f, 0.5f),
                                                     16.0f, bitdepthSliderValueToText, bitdepthSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_41_resamplerAudioProcessor::Plugex_41_resamplerAudioProcessor()
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
    srscaleParameter = parameters.getRawParameterValue("srscale");
    bitdepthParameter = parameters.getRawParameterValue("bitdepth");
}

Plugex_41_resamplerAudioProcessor::~Plugex_41_resamplerAudioProcessor()
{
}

//==============================================================================
const String Plugex_41_resamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_41_resamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_41_resamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_41_resamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_41_resamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_41_resamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_41_resamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_41_resamplerAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_41_resamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_41_resamplerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_41_resamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    sampleCount = 0;
    currentValue[0] = currentValue[1] = 0.0f;

    srscaleSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    srscaleSmoothed.setCurrentAndTargetValue(*srscaleParameter);
    bitdepthSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    bitdepthSmoothed.setCurrentAndTargetValue(*bitdepthParameter);
}

void Plugex_41_resamplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_41_resamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_41_resamplerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    srscaleSmoothed.setTargetValue(*srscaleParameter);
    bitdepthSmoothed.setTargetValue(*bitdepthParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float currentSrScale = srscaleSmoothed.getNextValue();
        float currentBitDepth = bitdepthSmoothed.getNextValue();

        float bitscl = powf(2.0f, currentBitDepth - 1.0f);
        float ibitscl = 1.0f / bitscl;

        float newsr = currentSampleRate * currentSrScale;
        float nsamps = (int)(currentSampleRate / newsr);

        sampleCount++;

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer (channel);
            if (sampleCount >= nsamps) {
                int tmp = (int)(channelData[i] * bitscl + 0.5);
                currentValue[channel] = tmp * ibitscl;
            }
            channelData[i] = currentValue[channel];
        }

        if (sampleCount >= nsamps) {
            sampleCount = 0;
        }
    }
}

//==============================================================================
bool Plugex_41_resamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_41_resamplerAudioProcessor::createEditor()
{
    return new Plugex_41_resamplerAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_41_resamplerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_41_resamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_41_resamplerAudioProcessor();
}
