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

static String gainSliderValueToText(float value) {
    float val = 20.0f * log10f(jmax(0.001f, value));
    return String(val, 2) + String(" dB");
}

static float gainSliderTextToValue(const String& text) {
    float val = jlimit(-60.0f, 18.0f, text.getFloatValue());
    return powf(10.0f, val * 0.05f);
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("gain"), String("Gain"), String(),
                                                     NormalisableRange<float>(0.001f, 7.94f, 0.001f, 0.3f),
                                                     1.0f, gainSliderValueToText, gainSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_02_amplitudeDbAudioProcessor::Plugex_02_amplitudeDbAudioProcessor()
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
    gainParameter = parameters.getRawParameterValue("gain");
}

Plugex_02_amplitudeDbAudioProcessor::~Plugex_02_amplitudeDbAudioProcessor()
{
}

//==============================================================================
const String Plugex_02_amplitudeDbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_02_amplitudeDbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_02_amplitudeDbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_02_amplitudeDbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_02_amplitudeDbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_02_amplitudeDbAudioProcessor::getNumPrograms()
{
return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_02_amplitudeDbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_02_amplitudeDbAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_02_amplitudeDbAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_02_amplitudeDbAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_02_amplitudeDbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    gainSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    gainSmoothed.setCurrentAndTargetValue(*gainParameter);
}

void Plugex_02_amplitudeDbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_02_amplitudeDbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_02_amplitudeDbAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    gainSmoothed.setTargetValue(*gainParameter);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int i = 0; i < buffer.getNumSamples(); i++) {
            channelData[i] *= gainSmoothed.getNextValue();
        }
    }
}

//==============================================================================
bool Plugex_02_amplitudeDbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_02_amplitudeDbAudioProcessor::createEditor()
{
    return new Plugex_02_amplitudeDbAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_02_amplitudeDbAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_02_amplitudeDbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_02_amplitudeDbAudioProcessor();
}
