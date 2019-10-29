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

static String driveSliderValueToText(float value) {
    return String(value, 3) + String(" x");
}

static float driveSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String cutoffSliderValueToText(float value) {
    return String(value, 2) + String(" Hz");
}

static float cutoffSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("drive"), String("Drive"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f),
                                                     0.5f, driveSliderValueToText, driveSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("cutoff"), String("Cutoff"), String(),
                                                     NormalisableRange<float>(20.0f, 18000.0f, 0.01f, 0.3f),
                                                     5000.0f, cutoffSliderValueToText, cutoffSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_15_arctanDistortionAudioProcessor::Plugex_15_arctanDistortionAudioProcessor()
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
    driveParameter = parameters.getRawParameterValue("drive");
    cutoffParameter = parameters.getRawParameterValue("cutoff");
}

Plugex_15_arctanDistortionAudioProcessor::~Plugex_15_arctanDistortionAudioProcessor()
{
}

//==============================================================================
const String Plugex_15_arctanDistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_15_arctanDistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_15_arctanDistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_15_arctanDistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_15_arctanDistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_15_arctanDistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_15_arctanDistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_15_arctanDistortionAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_15_arctanDistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_15_arctanDistortionAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_15_arctanDistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    driveSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    driveSmoothed.setCurrentAndTargetValue(*driveParameter);
    cutoffSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    cutoffSmoothed.setCurrentAndTargetValue(*cutoffParameter);

    for (int channel = 0; channel < 2; channel++) {
        lowpassFilter[channel].setup(sampleRate);
    }
}

void Plugex_15_arctanDistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_15_arctanDistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_15_arctanDistortionAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    driveSmoothed.setTargetValue(*driveParameter);
    cutoffSmoothed.setTargetValue(*cutoffParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float currentDrive = driveSmoothed.getNextValue();
        float currentCutoff = cutoffSmoothed.getNextValue();
        currentDrive = powf(1.0f - currentDrive, 3) * M_PI;
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            lowpassFilter[channel].setFreq(currentCutoff);
            auto* channelData = buffer.getWritePointer (channel);
            channelData[i] = atan2f(channelData[i], currentDrive) / atan2f(1.0f, currentDrive);
            channelData[i] = lowpassFilter[channel].process(channelData[i]);
        }
    }
}

//==============================================================================
bool Plugex_15_arctanDistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_15_arctanDistortionAudioProcessor::createEditor()
{
    return new Plugex_15_arctanDistortionAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_15_arctanDistortionAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_15_arctanDistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_15_arctanDistortionAudioProcessor();
}
