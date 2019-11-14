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

static String panSliderValueToText(float value) {
    return String(value, 2) + String(" L<>R");
}

static float panSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("pan"), String("Pan"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 0.01f, 1.f),
                                                     0.5f, panSliderValueToText, panSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("type"), String("Type"), String(),
                                                     NormalisableRange<float>(0.0f, 2.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_24_pannerAudioProcessor::Plugex_24_pannerAudioProcessor()
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
    panParameter = parameters.getRawParameterValue("pan");
    typeParameter = parameters.getRawParameterValue("type");
}

Plugex_24_pannerAudioProcessor::~Plugex_24_pannerAudioProcessor()
{
}

//==============================================================================
const String Plugex_24_pannerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_24_pannerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_24_pannerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_24_pannerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_24_pannerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_24_pannerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_24_pannerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_24_pannerAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_24_pannerAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_24_pannerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_24_pannerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    panSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    panSmoothed.setCurrentAndTargetValue(*panParameter);
}

void Plugex_24_pannerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_24_pannerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_24_pannerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    panSmoothed.setTargetValue(*panParameter);

    if (totalNumOutputChannels < 2) {
        return;
    }

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        float pan = panSmoothed.getNextValue();

        float sumValue = 0.0f;
        if (totalNumInputChannels == 1) {
            sumValue = buffer.getWritePointer(0)[i];
        } else {
            for (int channel = 0; channel < totalNumInputChannels; ++channel) {
                auto* channelData = buffer.getWritePointer (channel);
                sumValue += channelData[i];
            }
        }

        auto* channelDataL = buffer.getWritePointer(0);
        auto* channelDataR = buffer.getWritePointer(1);
        if (*typeParameter == 0.f) {
            channelDataL[i] = sumValue * (1.f - pan);
            channelDataR[i] = sumValue * pan;
        } else if (*typeParameter == 0.f) {
            channelDataL[i] = sumValue * sqrtf(1.f - pan);
            channelDataR[i] = sumValue * sqrtf(pan);
        } else {
            channelDataL[i] = sumValue * cosf(pan * M_PI / 2.f);
            channelDataR[i] = sumValue * sinf(pan * M_PI / 2.f);
        }
    }
}

//==============================================================================
bool Plugex_24_pannerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_24_pannerAudioProcessor::createEditor()
{
    return new Plugex_24_pannerAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_24_pannerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_24_pannerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_24_pannerAudioProcessor();
}
