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

static String depthSliderValueToText(float value) {
    return String(value, 2) + String(" x");
}

static float depthSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("freq"), String("Freq"), String(),
                                                     NormalisableRange<float>(0.001f, 20.0f, 0.001f, 0.3f),
                                                     1.0f, freqSliderValueToText, freqSliderTextToValue));
    parameters.push_back(std::make_unique<Parameter>(String("depth"), String("Depth"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f),
                                                     0.0f, depthSliderValueToText, depthSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_03_amplitudeLfoAudioProcessor::Plugex_03_amplitudeLfoAudioProcessor()
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

Plugex_03_amplitudeLfoAudioProcessor::~Plugex_03_amplitudeLfoAudioProcessor()
{
}

//==============================================================================
const String Plugex_03_amplitudeLfoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_03_amplitudeLfoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_03_amplitudeLfoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_03_amplitudeLfoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_03_amplitudeLfoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_03_amplitudeLfoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_03_amplitudeLfoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_03_amplitudeLfoAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_03_amplitudeLfoAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_03_amplitudeLfoAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_03_amplitudeLfoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    freqSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    freqSmoothed.setCurrentAndTargetValue(*freqParameter);
    depthSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    depthSmoothed.setCurrentAndTargetValue(*depthParameter);
    lfoPhase = 0.0f;
    lfoInc = *freqParameter / sampleRate;
}

void Plugex_03_amplitudeLfoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_03_amplitudeLfoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_03_amplitudeLfoAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    freqSmoothed.setTargetValue(*freqParameter);
    depthSmoothed.setTargetValue(*depthParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        lfoInc = freqSmoothed.getNextValue() / currentSampleRate;
        lfoPhase += lfoInc;
        if (lfoPhase >= 1.0f) {
            lfoPhase -= 1.0f;
        }
        float lfoValue = 1.0f - (sinf(M_PI * 2.0f * lfoPhase) * 0.5 + 0.5) * depthSmoothed.getNextValue();
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            auto* channelData = buffer.getWritePointer (channel);
            channelData[i] *= lfoValue;
        }
    }
}

//==============================================================================
bool Plugex_03_amplitudeLfoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_03_amplitudeLfoAudioProcessor::createEditor()
{
    return new Plugex_03_amplitudeLfoAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_03_amplitudeLfoAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_03_amplitudeLfoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_03_amplitudeLfoAudioProcessor();
}
