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

static String qSliderValueToText(float value) {
    return String(value, 2) + String(" Q");
}

static float qSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("freq"), String("Freq"), String(),
                                                     NormalisableRange<float>(20.0f, 18000.0f, 0.01f, 0.3f),
                                                     1000.0f, freqSliderValueToText, freqSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("q"), String("Q"), String(),
                                                     NormalisableRange<float>(1.0f, 50.0f, 0.01f, 0.5f),
                                                     1.0f, qSliderValueToText, qSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_10_butterworthBrAudioProcessor::Plugex_10_butterworthBrAudioProcessor()
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
    qParameter = parameters.getRawParameterValue("q");
}

Plugex_10_butterworthBrAudioProcessor::~Plugex_10_butterworthBrAudioProcessor()
{
}

//==============================================================================
const String Plugex_10_butterworthBrAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_10_butterworthBrAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_10_butterworthBrAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_10_butterworthBrAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_10_butterworthBrAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_10_butterworthBrAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_10_butterworthBrAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_10_butterworthBrAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_10_butterworthBrAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_10_butterworthBrAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_10_butterworthBrAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    memset(lastInputSample1, 0, sizeof(float) * 8);
    memset(lastInputSample2, 0, sizeof(float) * 8);
    memset(lastFilteredSample1, 0, sizeof(float) * 8);
    memset(lastFilteredSample2, 0, sizeof(float) * 8);

    freqSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    freqSmoothed.setCurrentAndTargetValue(*freqParameter);
    qSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    qSmoothed.setCurrentAndTargetValue(*qParameter);
}

void Plugex_10_butterworthBrAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_10_butterworthBrAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_10_butterworthBrAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    freqSmoothed.setTargetValue(*freqParameter);
    qSmoothed.setTargetValue(*qParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float cf = freqSmoothed.getNextValue();
        float bw = cf / qSmoothed.getNextValue();

        float piOnSr = M_PI / currentSampleRate;
        float c = tanf(piOnSr * bw);
        float d = 2.0f * cosf(2.0f * piOnSr * cf);

        float a0 = 1.0f / (1.0f + c);
        float a1 = -a0 * d;
        float a2 = a0;
        float b1 = a1;
        float b2 = a0 * (1.0f - c);
        
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer (channel);
            float filtered = a0 * channelData[i] + a1 * lastInputSample1[channel] + a2 * lastInputSample2[channel] - 
                             b1 * lastFilteredSample1[channel] - b2 * lastFilteredSample2[channel];
            lastInputSample2[channel] = lastInputSample1[channel];
            lastInputSample1[channel] = channelData[i];
            lastFilteredSample2[channel] = lastFilteredSample1[channel];
            channelData[i] = lastFilteredSample1[channel] = filtered;
        }
    }
}

//==============================================================================
bool Plugex_10_butterworthBrAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_10_butterworthBrAudioProcessor::createEditor()
{
    return new Plugex_10_butterworthBrAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_10_butterworthBrAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_10_butterworthBrAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_10_butterworthBrAudioProcessor();
}
