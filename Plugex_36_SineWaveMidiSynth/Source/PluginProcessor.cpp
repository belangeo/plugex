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

static const int numberOfVoices = 10;

//==============================================================================
SineWaveVoice::SineWaveVoice() {
    oscillator.setup(getSampleRate());
}

bool SineWaveVoice::canPlaySound(SynthesiserSound *sound) {
    return dynamic_cast<SineWaveSound *> (sound) != nullptr;
}

void SineWaveVoice::startNote(int midiNoteNumber, float velocity,
                              SynthesiserSound *, int /*currentPitchWheelPosition*/) {
    level = velocity * 0.15;
    tailOff = 0.0;
    oscillator.setFreq(MidiMessage::getMidiNoteInHertz (midiNoteNumber));
}

void SineWaveVoice::stopNote(float /*velocity*/, bool allowTailOff) {
    if (allowTailOff) {
        if (tailOff == 0.0)
            tailOff = 1.0;
    } else {
        clearCurrentNote();
    }
}

void SineWaveVoice::renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) {
    if (tailOff > 0.0) {
        while (--numSamples >= 0) {
            auto currentSample = oscillator.process() * level * tailOff;

            for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                outputBuffer.addSample(i, startSample, currentSample);

            ++startSample;

            tailOff *= 0.99;

            if (tailOff <= 0.005) {
                clearCurrentNote();
                break;
            }
        }
    } else {
        while (--numSamples >= 0) {
            auto currentSample = oscillator.process() * level;

            for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                outputBuffer.addSample (i, startSample, currentSample);

            ++startSample;
        }
    }
}

//==============================================================================
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
Plugex_36_sineWaveMidiSynthAudioProcessor::Plugex_36_sineWaveMidiSynthAudioProcessor()
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

    for (auto i = 0; i < numberOfVoices; ++i)
        synthesiser.addVoice(new SineWaveVoice());

    synthesiser.addSound(new SineWaveSound());

    gainParameter = parameters.getRawParameterValue("gain");
}

Plugex_36_sineWaveMidiSynthAudioProcessor::~Plugex_36_sineWaveMidiSynthAudioProcessor()
{
}

//==============================================================================
const String Plugex_36_sineWaveMidiSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_36_sineWaveMidiSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_36_sineWaveMidiSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_36_sineWaveMidiSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_36_sineWaveMidiSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_36_sineWaveMidiSynthAudioProcessor::getNumPrograms()
{
return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_36_sineWaveMidiSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_36_sineWaveMidiSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_36_sineWaveMidiSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_36_sineWaveMidiSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_36_sineWaveMidiSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    keyboardState.reset();
    synthesiser.setCurrentPlaybackSampleRate(sampleRate);
}

void Plugex_36_sineWaveMidiSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_36_sineWaveMidiSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_36_sineWaveMidiSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    synthesiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    buffer.applyGainRamp(0, buffer.getNumSamples(), lastGain, *gainParameter);
    lastGain = *gainParameter;
}

//==============================================================================
bool Plugex_36_sineWaveMidiSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_36_sineWaveMidiSynthAudioProcessor::createEditor()
{
    return new Plugex_36_sineWaveMidiSynthAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_36_sineWaveMidiSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_36_sineWaveMidiSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_36_sineWaveMidiSynthAudioProcessor();
}
