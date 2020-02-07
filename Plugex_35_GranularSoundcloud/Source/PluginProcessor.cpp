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

static String densitySliderValueToText(float value) {
    return String(value, 2) + String(" x");
}

static float densitySliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String sliderValueToText(float value) {
    return String(value, 2) + String(" %");
}

static float sliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("active"), String("Active"), String(),
                                                     NormalisableRange<float>(0.f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("density"), String("Density"), String(),
                                                     NormalisableRange<float>(1.f, 250.f, 0.01f, .3f),
                                                     50.f, densitySliderValueToText, densitySliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("rndpit"), String("RndPit"), String(),
                                                     NormalisableRange<float>(0.f, 100.f, 0.01f, 1.f),
                                                     50.f, sliderValueToText, sliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("rndpos"), String("RndPos"), String(),
                                                     NormalisableRange<float>(0.f, 100.f, 0.01f, 1.f),
                                                     50.f, sliderValueToText, sliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("rnddur"), String("RndDur"), String(),
                                                     NormalisableRange<float>(0.f, 100.f, 0.01f, 1.f),
                                                     50.f, sliderValueToText, sliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_35_granularSoundcloudAudioProcessor::Plugex_35_granularSoundcloudAudioProcessor()
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
    rndpitParameter = parameters.getRawParameterValue("rndpit");
    rndposParameter = parameters.getRawParameterValue("rndpos");
    rnddurParameter = parameters.getRawParameterValue("rnddur");
}

Plugex_35_granularSoundcloudAudioProcessor::~Plugex_35_granularSoundcloudAudioProcessor()
{
}

//==============================================================================
const String Plugex_35_granularSoundcloudAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_35_granularSoundcloudAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_35_granularSoundcloudAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_35_granularSoundcloudAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_35_granularSoundcloudAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_35_granularSoundcloudAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_35_granularSoundcloudAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_35_granularSoundcloudAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_35_granularSoundcloudAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_35_granularSoundcloudAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_35_granularSoundcloudAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_sampleRate = sampleRate;
    granulator[0].setup(sampleRate, 2.f);
    granulator[1].setup(sampleRate, 2.f);
    granulator[0].setRecording(false);
    granulator[1].setRecording(false);
    portLastSample = *activeParameter;
    densitySmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    densitySmoothed.setCurrentAndTargetValue(*densityParameter);
    rndpitSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    rndpitSmoothed.setCurrentAndTargetValue(*rndpitParameter);
    rndposSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    rndposSmoothed.setCurrentAndTargetValue(*rndposParameter);
    rnddurSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    rnddurSmoothed.setCurrentAndTargetValue(*rnddurParameter);
}

void Plugex_35_granularSoundcloudAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_35_granularSoundcloudAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_35_granularSoundcloudAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    densitySmoothed.setTargetValue(*densityParameter);
    rndpitSmoothed.setTargetValue(*rndpitParameter);
    rndposSmoothed.setTargetValue(*rndposParameter);
    rnddurSmoothed.setTargetValue(*rnddurParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float rndpit = rndpitSmoothed.getNextValue() * 0.019f;
        float rndpos = rndposSmoothed.getNextValue() * 0.019f;
        float rnddur = rnddurSmoothed.getNextValue() * 0.019f;

        float density = densitySmoothed.getNextValue();
        float pitch = 1.f * (jitterRandom.nextFloat() * rndpit - (rndpit * 0.5f) + 1.f);
        float position = 0.5f * (jitterRandom.nextFloat() * rndpos - (rndpos * 0.5f) + 1.f);
        float duration = 0.2f * (jitterRandom.nextFloat() * rnddur - (rnddur * 0.5f) + 1.f);

        bool activateRecording = (bool)*activeParameter && !isActive;

        if (!isActive || granulator[0].getIsRecording()) {
            portLastSample = 0.f + (portLastSample - 0.f) * 0.9999;
        } else {
            portLastSample = 1.f + (portLastSample - 1.f) * 0.9999;
        }

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float cloudSample = 0.f;
            auto* channelData = buffer.getWritePointer (channel);
            if (activateRecording) {
                granulator[channel].setRecording(true);
            }
            granulator[channel].setDensity(density);
            granulator[channel].setPitch(pitch);
            granulator[channel].setDuration(duration);
            granulator[channel].setPosition(position);
            if (isActive)
                cloudSample = granulator[channel].process(channelData[i]);
            channelData[i] = channelData[i] + (cloudSample - channelData[i]) * portLastSample;
        }
        isActive = (bool)*activeParameter;
    }
}

//==============================================================================
bool Plugex_35_granularSoundcloudAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_35_granularSoundcloudAudioProcessor::createEditor()
{
    return new Plugex_35_granularSoundcloudAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_35_granularSoundcloudAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_35_granularSoundcloudAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_35_granularSoundcloudAudioProcessor();
}
