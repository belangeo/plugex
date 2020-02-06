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

static String densitySliderValueToText(float value) {
    return String(value, 2) + String(" x");
}

static float densitySliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String pitchSliderValueToText(float value) {
    return String(value, 3) + String(" x");
}

static float pitchSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String durationSliderValueToText(float value) {
    return String(value, 3) + String(" sec");
}

static float durationSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String jitterSliderValueToText(float value) {
    return String(value, 3) + String(" %");
}

static float jitterSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("active"), String("Active"), String(),
                                                     NormalisableRange<float>(0.f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("density"), String("Density"), String(),
                                                     NormalisableRange<float>(1.f, 500.0f, 0.01f, 0.3f),
                                                     50.0f, densitySliderValueToText, densitySliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("pitch"), String("Pitch"), String(),
                                                     NormalisableRange<float>(0.1f, 2.0f, 0.001f, 1.0f),
                                                     1.0f, pitchSliderValueToText, pitchSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("duration"), String("Duration"), String(),
                                                     NormalisableRange<float>(0.001f, 0.25f, 0.001f, 0.5f),
                                                     0.1f, durationSliderValueToText, durationSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("jitter"), String("Jitter"), String(),
                                                     NormalisableRange<float>(0.f, 100.0f, 0.001f, 0.3f),
                                                     5.0f, jitterSliderValueToText, jitterSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_33_granularFreezeAudioProcessor::Plugex_33_granularFreezeAudioProcessor()
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
    activeParameter = parameters.getRawParameterValue("active");
    densityParameter = parameters.getRawParameterValue("density");
    pitchParameter = parameters.getRawParameterValue("pitch");
    durationParameter = parameters.getRawParameterValue("duration");
    jitterParameter = parameters.getRawParameterValue("jitter");
}

Plugex_33_granularFreezeAudioProcessor::~Plugex_33_granularFreezeAudioProcessor()
{
}

//==============================================================================
const String Plugex_33_granularFreezeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_33_granularFreezeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_33_granularFreezeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_33_granularFreezeAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_33_granularFreezeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_33_granularFreezeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_33_granularFreezeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_33_granularFreezeAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_33_granularFreezeAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_33_granularFreezeAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_33_granularFreezeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    granulator[0].setup(sampleRate, 0.5);
    granulator[1].setup(sampleRate, 0.5);
    granulator[0].setRecording(false);
    granulator[1].setRecording(false);
    portLastSample = *activeParameter;
    densitySmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    densitySmoothed.setCurrentAndTargetValue(*densityParameter);
    pitchSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    pitchSmoothed.setCurrentAndTargetValue(*pitchParameter);
    durationSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    durationSmoothed.setCurrentAndTargetValue(*durationParameter);
    jitterSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    jitterSmoothed.setCurrentAndTargetValue(*jitterParameter);
}

void Plugex_33_granularFreezeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_33_granularFreezeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_33_granularFreezeAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    densitySmoothed.setTargetValue(*densityParameter);
    pitchSmoothed.setTargetValue(*pitchParameter);
    durationSmoothed.setTargetValue(*durationParameter);
    jitterSmoothed.setTargetValue(*jitterParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float jitter = jitterSmoothed.getNextValue() * 0.01f;
        float density = densitySmoothed.getNextValue() + jitterRandom.nextFloat() * 10.f * jitter;
        float pitch = pitchSmoothed.getNextValue() * ((jitterRandom.nextFloat() - 0.5f) * 0.25f * jitter + 1.f);
        float duration = durationSmoothed.getNextValue() * ((jitterRandom.nextFloat() - 0.5f) * 0.25f * jitter + 1.f);
        float position = jitterRandom.nextFloat() * 0.8f * jitter;
        float deviation = jitterRandom.nextFloat() * 0.2f * jitter;
        
        bool activateRecording = (bool)*activeParameter && !isActive;

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float freezeSample = 0.f;
            portLastSample = *activeParameter + (portLastSample - *activeParameter) * 0.9999;
            auto* channelData = buffer.getWritePointer (channel);
            if (activateRecording)
                granulator[channel].setRecording(true);
            granulator[channel].setDensity(density);
            granulator[channel].setPitch(pitch);
            granulator[channel].setDuration(duration);
            granulator[channel].setPosition(position);
            if (isActive)
                freezeSample = granulator[channel].process(channelData[i]);
            channelData[i] = channelData[i] + (freezeSample - channelData[i]) * portLastSample;
        }
        isActive = (bool)*activeParameter;
    }
}

//==============================================================================
bool Plugex_33_granularFreezeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_33_granularFreezeAudioProcessor::createEditor()
{
    return new Plugex_33_granularFreezeAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_33_granularFreezeAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_33_granularFreezeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_33_granularFreezeAudioProcessor();
}
