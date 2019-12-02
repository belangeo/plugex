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

static String threshSliderValueToText(float value) {
    return String(value, 2) + String(" dB");
}

static float threshSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String risetimeSliderValueToText(float value) {
    return String(value, 2) + String(" ms");
}

static float risetimeSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String lookaheadSliderValueToText(float value) {
    return String(value, 2) + String(" ms");
}

static float lookaheadSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("thresh"), String("Tresh"), String(),
                                                     NormalisableRange<float>(-90.0f, 0.0f, 0.01f, 1.0f),
                                                     -60.0f, threshSliderValueToText, threshSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("risetime"), String("Risetime"), String(),
                                                     NormalisableRange<float>(0.01f, 500.0f, 0.01f, 0.3f),
                                                     5.0f, risetimeSliderValueToText, risetimeSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("falltime"), String("Falltime"), String(),
                                                     NormalisableRange<float>(0.01f, 500.0f, 0.01f, 0.3f),
                                                     10.0f, risetimeSliderValueToText, risetimeSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("lookahead"), String("LookAhead"), String(),
                                                     NormalisableRange<float>(0.01f, 10.0f, 0.01f, 1.0f),
                                                     5.0f, lookaheadSliderValueToText, lookaheadSliderTextToValue));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_28_gateAudioProcessor::Plugex_28_gateAudioProcessor()
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
    threshParameter = parameters.getRawParameterValue("thresh");
    risetimeParameter = parameters.getRawParameterValue("risetime");
    falltimeParameter = parameters.getRawParameterValue("falltime");
    lookaheadParameter = parameters.getRawParameterValue("lookahead");
}

Plugex_28_gateAudioProcessor::~Plugex_28_gateAudioProcessor()
{
}

//==============================================================================
const String Plugex_28_gateAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_28_gateAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_28_gateAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_28_gateAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_28_gateAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_28_gateAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_28_gateAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_28_gateAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_28_gateAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_28_gateAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_28_gateAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    threshSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    threshSmoothed.setCurrentAndTargetValue(*threshParameter);
    risetimeSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    risetimeSmoothed.setCurrentAndTargetValue(*risetimeParameter);
    falltimeSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    falltimeSmoothed.setCurrentAndTargetValue(*falltimeParameter);
    lookaheadSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    lookaheadSmoothed.setCurrentAndTargetValue(*lookaheadParameter);

    for (int channel = 0; channel < 2; channel++) {
        lowpassFilter[channel].setup(currentSampleRate);
        gateFilter[channel].setup(currentSampleRate);
        lookaheadDelay[channel].setup(0.015, currentSampleRate);
    }
}

void Plugex_28_gateAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_28_gateAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_28_gateAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    threshSmoothed.setTargetValue(*threshParameter);
    risetimeSmoothed.setTargetValue(*risetimeParameter);
    falltimeSmoothed.setTargetValue(*falltimeParameter);
    lookaheadSmoothed.setTargetValue(*lookaheadParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float thresh = powf(20.0f, threshSmoothed.getNextValue() * 0.05f);
        float risetime = risetimeSmoothed.getNextValue() * 0.001f;
        float falltime = falltimeSmoothed.getNextValue() * 0.001f;
        float lookahead = lookaheadSmoothed.getNextValue() * 0.001f;

        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            auto* channelData = buffer.getWritePointer (channel);
            float rectified = channelData[i] < 0.0f ? -channelData[i] : channelData[i];
            lowpassFilter[channel].setFreq(10.0f);
            float follower = lowpassFilter[channel].process(rectified);

            float gate;
            if (follower >= thresh) {
                gateFilter[channel].setFreq(1.0f / risetime);
                gate = gateFilter[channel].process(1.0f);
            } else {
                gateFilter[channel].setFreq(1.0f / falltime);
                gate = gateFilter[channel].process(0.0f);
            }

            float delayedSample = lookaheadDelay[channel].read(lookahead);
            lookaheadDelay[channel].write(channelData[i]);
            channelData[i] = delayedSample * gate;
        }
    }
}

//==============================================================================
bool Plugex_28_gateAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_28_gateAudioProcessor::createEditor()
{
    return new Plugex_28_gateAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_28_gateAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_28_gateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_28_gateAudioProcessor();
}
