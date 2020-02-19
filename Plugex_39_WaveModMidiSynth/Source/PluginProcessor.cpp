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
MySynthesiserVoice::MySynthesiserVoice() {
    lfo.setup(getSampleRate());
    lfo.setSharp(1.f);
    oscillatorLeft.setup(getSampleRate());
    oscillatorRight.setup(getSampleRate());
    envelope.setSampleRate(getSampleRate());
}

bool MySynthesiserVoice::canPlaySound(SynthesiserSound *sound) {
    return dynamic_cast<MySynthesiserSound *> (sound) != nullptr;
}

void MySynthesiserVoice::startNote(int midiNoteNumber, float velocity,
                              SynthesiserSound *, int /*currentPitchWheelPosition*/) {
    noteFreq = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    level = velocity * 0.15;
    envelope.noteOn();
    lfo.reset();
    oscillatorLeft.setFreq(noteFreq);
    oscillatorLeft.setSharp(noteSharp);
    oscillatorRight.setFreq(noteFreq);
    oscillatorRight.setSharp(noteSharp);
}

void MySynthesiserVoice::stopNote(float /*velocity*/, bool allowTailOff) {
    envelope.noteOff();
}

void MySynthesiserVoice::renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) {
    while (--numSamples >= 0) {
        auto envAmp = envelope.getNextSample();
        auto lfoValue = lfo.process() * lfoDepth;

        if (freqSwitch) {
            if (freqSwitchInv) {
                oscillatorLeft.setFreq(noteFreq * (1.f + -lfoValue * 0.25f));
                oscillatorRight.setFreq(noteFreq * 1.003 * (1.f + -lfoValue * 0.25f));
            } else {
                oscillatorLeft.setFreq(noteFreq * (1.f + lfoValue * 0.25f));
                oscillatorRight.setFreq(noteFreq * 1.003 * (1.f + lfoValue * 0.25f));
            }
        }

        if (sharpSwitch) {
            if (sharpSwitchInv) {
                oscillatorLeft.setSharp(noteSharp * (1.f + -lfoValue));
                oscillatorRight.setSharp(noteSharp * (1.f + -lfoValue));
            } else {
                oscillatorLeft.setSharp(noteSharp * (1.f + lfoValue));
                oscillatorRight.setSharp(noteSharp * (1.f + lfoValue));
            }
        }

        float currentSample, currentSample2;
        if (gainSwitch) {
            if (gainSwitchInv) {
                currentSample = oscillatorLeft.process() * level * envAmp * (1.f + -lfoValue);
                currentSample2 = oscillatorRight.process() * level * envAmp * (1.f + -lfoValue);
            } else {
                currentSample = oscillatorLeft.process() * level * envAmp * (1.f + lfoValue);
                currentSample2 = oscillatorRight.process() * level * envAmp * (1.f + lfoValue);
            }
        } else {
            currentSample = oscillatorLeft.process() * level * envAmp;
            currentSample2 = oscillatorRight.process() * level * envAmp;
        }

        for (auto i = outputBuffer.getNumChannels(); --i >= 0;) {
            if (i && stereoSwitch)
                outputBuffer.addSample(i, startSample, currentSample2);
            else
                outputBuffer.addSample(i, startSample, currentSample);
        }

        ++startSample;

        if (envAmp <= 0.0) {
            clearCurrentNote();
            break;
        }
    }
}

void MySynthesiserVoice::setEnvelopeParameters(ADSR::Parameters params) {
    envelope.setParameters(params);
}

void MySynthesiserVoice::setWavetypeParameter(int type) {
    oscillatorLeft.setWavetype(type);
    oscillatorRight.setWavetype(type);
}

void MySynthesiserVoice::setSharpParameter(float sharp) {
    noteSharp = sharp;
}

void MySynthesiserVoice::setStereoToggleParameter(int state) {
    stereoSwitch = state;
}

void MySynthesiserVoice::setLFOWavetypeParameter(int type) {
    lfo.setWavetype(type);
}

void MySynthesiserVoice::setLFOFreqParameter(float freq) {
    lfo.setFreq(freq);
}

void MySynthesiserVoice::setLFODepthParameter(float depth) {
    lfoDepth = depth;
}

void MySynthesiserVoice::setLFORoutingParameters(int freq, int freqInv, int sharp, int sharpInv, int gain, int gainInv) {
    freqSwitch = freq;
    freqSwitchInv = freqInv;
    sharpSwitch = sharp;
    sharpSwitchInv = sharpInv;
    gainSwitch = gain;
    gainSwitchInv = gainInv;
}

//==============================================================================
void MySynthesiser::setEnvelopeParameters(ADSR::Parameters params) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setEnvelopeParameters(params);
}

void MySynthesiser::setWavetypeParameter(int type) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setWavetypeParameter(type);
}

void MySynthesiser::setSharpParameter(float sharp) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setSharpParameter(sharp);
}

void MySynthesiser::setStereoToggleParameter(int state) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setStereoToggleParameter(state);
}

void MySynthesiser::setLFOWavetypeParameter(int type) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setLFOWavetypeParameter(type);
}

void MySynthesiser::setLFOFreqParameter(float freq) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setLFOFreqParameter(freq);
}

void MySynthesiser::setLFODepthParameter(float depth) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setLFODepthParameter(depth);
}

void MySynthesiser::setLFORoutingParameters(int freq, int freqInv, int sharp, int sharpInv, int gain, int gainInv) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setLFORoutingParameters(freq, freqInv, sharp, sharpInv, gain, gainInv);
}

//==============================================================================
static String freqSliderValueToText(float value) {
    return String(value, 3) + String(" Hz");
}

static float freqSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String secondSliderValueToText(float value) {
    return String(value, 3) + String(" sec");
}

static float secondSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String sharpSliderValueToText(float value) {
    return String(value, 4) + String(" x");
}

static float sharpSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

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

    parameters.push_back(std::make_unique<Parameter>(String("attack"), String("Attack"), String(),
                                                     NormalisableRange<float>(0.001f, 1.f, 0.001f, 0.5f),
                                                     0.005f, secondSliderValueToText, secondSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("decay"), String("Decay"), String(),
                                                     NormalisableRange<float>(0.001f, 1.f, 0.001f, 0.5f),
                                                     0.2f, secondSliderValueToText, secondSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("sustain"), String("Sustain"), String(),
                                                     NormalisableRange<float>(0.001f, 1.f, 0.001f, 0.3f),
                                                     0.708f, gainSliderValueToText, gainSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("release"), String("Release"), String(),
                                                     NormalisableRange<float>(0.001f, 1.f, 0.001f, 0.5f),
                                                     0.25f, secondSliderValueToText, secondSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("lfotype"), String("LFOType"), String(),
                                                     NormalisableRange<float>(0.0f, 7.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfofreq"), String("LFOFreq"), String(),
                                                     NormalisableRange<float>(0.01f, 16.f, 0.001f, 0.3f),
                                                     0.5f, freqSliderValueToText, freqSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("lfodepth"), String("LFODepth"), String(),
                                                     NormalisableRange<float>(0.f, 1.f, 0.001f, 1.f),
                                                     0.1f, sharpSliderValueToText, sharpSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("lfoRouteFreq"), String("LFORouteFreq"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfoRouteFreqInv"), String("LFORouteFreqInv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfoRouteSharp"), String("LFORouteSharp"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfoRouteSharpInv"), String("LFORouteSharpInv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfoRouteGain"), String("LFORouteGain"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfoRouteGainInv"), String("LFORouteGainInv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("type"), String("Type"), String(),
                                                     NormalisableRange<float>(0.0f, 7.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("sharp"), String("Sharp"), String(),
                                                     NormalisableRange<float>(0.f, 1.f, 0.001f, 0.5f),
                                                     0.5f, sharpSliderValueToText, sharpSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("gain"), String("Gain"), String(),
                                                     NormalisableRange<float>(0.001f, 7.94f, 0.001f, 0.3f),
                                                     1.0f, gainSliderValueToText, gainSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("stereo"), String("Stere"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_39_waveModMidiSynthAudioProcessor::Plugex_39_waveModMidiSynthAudioProcessor()
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
        synthesiser.addVoice(new MySynthesiserVoice());

    synthesiser.addSound(new MySynthesiserSound());

    attackParameter = parameters.getRawParameterValue("attack");
    decayParameter = parameters.getRawParameterValue("decay");
    sustainParameter = parameters.getRawParameterValue("sustain");
    releaseParameter = parameters.getRawParameterValue("release");
    lfotypeParameter = parameters.getRawParameterValue("lfotype");
    lfofreqParameter = parameters.getRawParameterValue("lfofreq");
    lfodepthParameter = parameters.getRawParameterValue("lfodepth");
    lfoRouteFreqParameter = parameters.getRawParameterValue("lfoRouteFreq");
    lfoRouteFreqInvParameter = parameters.getRawParameterValue("lfoRouteFreqInv");
    lfoRouteSharpParameter = parameters.getRawParameterValue("lfoRouteSharp");
    lfoRouteSharpInvParameter = parameters.getRawParameterValue("lfoRouteSharpInv");
    lfoRouteGainParameter = parameters.getRawParameterValue("lfoRouteGain");
    lfoRouteGainInvParameter = parameters.getRawParameterValue("lfoRouteGainInv");
    typeParameter = parameters.getRawParameterValue("type");
    sharpParameter = parameters.getRawParameterValue("sharp");
    gainParameter = parameters.getRawParameterValue("gain");
    stereoParameter = parameters.getRawParameterValue("stereo");
}

Plugex_39_waveModMidiSynthAudioProcessor::~Plugex_39_waveModMidiSynthAudioProcessor()
{
}

//==============================================================================
const String Plugex_39_waveModMidiSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_39_waveModMidiSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_39_waveModMidiSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_39_waveModMidiSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_39_waveModMidiSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_39_waveModMidiSynthAudioProcessor::getNumPrograms()
{
return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_39_waveModMidiSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_39_waveModMidiSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_39_waveModMidiSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_39_waveModMidiSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_39_waveModMidiSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    keyboardState.reset();
    synthesiser.setCurrentPlaybackSampleRate(sampleRate);
}

void Plugex_39_waveModMidiSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_39_waveModMidiSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_39_waveModMidiSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    synthesiser.setEnvelopeParameters(ADSR::Parameters {*attackParameter, *decayParameter, *sustainParameter, *releaseParameter});
    synthesiser.setWavetypeParameter((int)*typeParameter);
    synthesiser.setSharpParameter(*sharpParameter);
    synthesiser.setStereoToggleParameter((int)*stereoParameter);
    synthesiser.setLFOWavetypeParameter((int)*lfotypeParameter);
    synthesiser.setLFOFreqParameter(*lfofreqParameter);
    synthesiser.setLFODepthParameter(*lfodepthParameter);
    synthesiser.setLFORoutingParameters(*lfoRouteFreqParameter, *lfoRouteFreqInvParameter,
                                        *lfoRouteSharpParameter, *lfoRouteSharpInvParameter,
                                        *lfoRouteGainParameter, *lfoRouteGainInvParameter);

    synthesiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    buffer.applyGainRamp(0, buffer.getNumSamples(), lastGain, *gainParameter);
    lastGain = *gainParameter;
}

//==============================================================================
bool Plugex_39_waveModMidiSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_39_waveModMidiSynthAudioProcessor::createEditor()
{
    return new Plugex_39_waveModMidiSynthAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_39_waveModMidiSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_39_waveModMidiSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_39_waveModMidiSynthAudioProcessor();
}
