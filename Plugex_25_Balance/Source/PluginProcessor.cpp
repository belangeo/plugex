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

static String balSliderValueToText(float value) {
    return String(value, 2) + String(" L<>R");
}

static float balSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("bal"), String("Balance"), String(),
                                                     NormalisableRange<float>(-1.0f, 1.0f),
                                                     0.0f, balSliderValueToText, balSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_25_balanceAudioProcessor::Plugex_25_balanceAudioProcessor()
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
    balParameter = parameters.getRawParameterValue("bal");
}

Plugex_25_balanceAudioProcessor::~Plugex_25_balanceAudioProcessor()
{
}

//==============================================================================
const String Plugex_25_balanceAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_25_balanceAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_25_balanceAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_25_balanceAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_25_balanceAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_25_balanceAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_25_balanceAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_25_balanceAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_25_balanceAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_25_balanceAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_25_balanceAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    balSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    balSmoothed.setCurrentAndTargetValue(*balParameter);
}

void Plugex_25_balanceAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_25_balanceAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_25_balanceAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    balSmoothed.setTargetValue(*balParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        float balance = balSmoothed.getNextValue();
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            auto* channelData = buffer.getWritePointer (channel);
            float gain;
            if (channel % 2 == 0) {
                gain = balance < 0.0f ? 1.f : cosf(balance * M_PI / 2.f);
            } else {
                gain = balance < 0.0f ? cosf(balance * M_PI / 2.f) : 1.f;
            }
            channelData[i] *= gain;
        }
    }
}

//==============================================================================
bool Plugex_25_balanceAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_25_balanceAudioProcessor::createEditor()
{
    return new Plugex_25_balanceAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_25_balanceAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_25_balanceAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_25_balanceAudioProcessor();
}
