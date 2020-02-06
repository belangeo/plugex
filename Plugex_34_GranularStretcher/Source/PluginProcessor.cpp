/*******************************************************************************
* Plugex - PLUGin EXamples
*
* Plugex est une série de plugiciels auto-documentés permettant une étude 
* autonome du développement de plugiciels avec JUCE ainsi que des bases du
* traitement de signal audio avec le langage C++.
*
* © Olivier Bélanger 2020
*
*******************************************************************************/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

static String durationSliderValueToText(float value) {
    return String(value, 2) + String(" sec");
}

static float durationSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String pitchSliderValueToText(float value) {
    return String(value, 3) + String(" x");
}

static float pitchSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String speedSliderValueToText(float value) {
    return String(value, 2) + String(" x");
}

static float speedSliderTextToValue(const String& text) {
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

    parameters.push_back(std::make_unique<Parameter>(String("duration"), String("Duration"), String(),
                                                     NormalisableRange<float>(0.25f, 10.f, 0.01f, 0.5f),
                                                     1.f, durationSliderValueToText, durationSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("pitch"), String("Pitch"), String(),
                                                     NormalisableRange<float>(0.1f, 2.0f, 0.001f, 1.0f),
                                                     1.0f, pitchSliderValueToText, pitchSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("speed"), String("Speed"), String(),
                                                     NormalisableRange<float>(.1f, 4.0f, 0.01f, 0.3f),
                                                     1.0f, speedSliderValueToText, speedSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("jitter"), String("Jitter"), String(),
                                                     NormalisableRange<float>(0.f, 100.0f, 0.001f, 0.3f),
                                                     5.0f, jitterSliderValueToText, jitterSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_34_granularStretcherAudioProcessor::Plugex_34_granularStretcherAudioProcessor()
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
    durationParameter = parameters.getRawParameterValue("duration");
    pitchParameter = parameters.getRawParameterValue("pitch");
    speedParameter = parameters.getRawParameterValue("speed");
    jitterParameter = parameters.getRawParameterValue("jitter");
}

Plugex_34_granularStretcherAudioProcessor::~Plugex_34_granularStretcherAudioProcessor()
{
}

//==============================================================================
const String Plugex_34_granularStretcherAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_34_granularStretcherAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_34_granularStretcherAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_34_granularStretcherAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_34_granularStretcherAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_34_granularStretcherAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_34_granularStretcherAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_34_granularStretcherAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_34_granularStretcherAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_34_granularStretcherAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_34_granularStretcherAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_sampleRate = sampleRate;
    granulator[0].setup(sampleRate, 10.f);
    granulator[1].setup(sampleRate, 10.f);
    granulator[0].setRecording(false);
    granulator[1].setRecording(false);
    portLastSample = *activeParameter;
    durationSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    durationSmoothed.setCurrentAndTargetValue(*durationParameter);
    pitchSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    pitchSmoothed.setCurrentAndTargetValue(*pitchParameter);
    speedSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    speedSmoothed.setCurrentAndTargetValue(*speedParameter);
    jitterSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    jitterSmoothed.setCurrentAndTargetValue(*jitterParameter);
}

void Plugex_34_granularStretcherAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_34_granularStretcherAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_34_granularStretcherAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    durationSmoothed.setTargetValue(*durationParameter);
    pitchSmoothed.setTargetValue(*pitchParameter);
    speedSmoothed.setTargetValue(*speedParameter);
    jitterSmoothed.setTargetValue(*jitterParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float jitter = jitterSmoothed.getNextValue() * 0.01f;
        float duration = durationSmoothed.getNextValue();
        float grainDuration = 0.15 * ((jitterRandom.nextFloat() - 0.5f) * 0.05f * jitter + 1.f);
        float pitch = pitchSmoothed.getNextValue() * ((jitterRandom.nextFloat() - 0.5f) * 0.05f * jitter + 1.f);
        float deviation = jitterRandom.nextFloat() * 0.05f * jitter;
        float density = 100.f + jitterRandom.nextFloat() * 5.f * jitter;

        float speed = speedSmoothed.getNextValue() * ((jitterRandom.nextFloat() - 0.5f) * 0.05f * jitter + 1.f);
        float position = readerIndex * ((jitterRandom.nextFloat() - 0.5f) * 0.05f * jitter + 1.f);
        readerIndex += readerBaseInc * speed;
        if (readerIndex >= 1.f)
            readerIndex -= 1.f;

        bool activateRecording = (bool)*activeParameter && !isActive;
        if (activateRecording)
            readerBaseInc = (1.f / duration) / m_sampleRate;

        if (!isActive || granulator[0].getIsRecording()) {
            portLastSample = 0.f + (portLastSample - 0.f) * 0.9999;
        } else {
            portLastSample = 1.f + (portLastSample - 1.f) * 0.9999;
        }

        if (isRecording && !granulator[0].getIsRecording())
            readerIndex = 0.f;

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float stretchSample = 0.f;
            auto* channelData = buffer.getWritePointer (channel);
            if (activateRecording) {
                granulator[channel].setRecordingSize(duration);
                granulator[channel].setRecording(true);
            }
            granulator[channel].setDensity(density);
            granulator[channel].setPitch(pitch);
            granulator[channel].setDuration(grainDuration);
            granulator[channel].setPosition(position);
            granulator[channel].setDeviation(deviation);
            if (isActive)
                stretchSample = granulator[channel].process(channelData[i]);
            channelData[i] = channelData[i] + (stretchSample - channelData[i]) * portLastSample;
        }
        isActive = (bool)*activeParameter;
        isRecording = granulator[0].getIsRecording();
    }
}

//==============================================================================
bool Plugex_34_granularStretcherAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_34_granularStretcherAudioProcessor::createEditor()
{
    return new Plugex_34_granularStretcherAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_34_granularStretcherAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_34_granularStretcherAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_34_granularStretcherAudioProcessor();
}
