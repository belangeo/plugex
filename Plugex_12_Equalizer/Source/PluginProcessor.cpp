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

static String boostSliderValueToText(float value) {
    return String(value, 2) + String(" dB");
}

static float boostSliderTextToValue(const String& text) {
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

    parameters.push_back(std::make_unique<Parameter>(String("boost"), String("Boost"), String(),
                                                     NormalisableRange<float>(-24.0f, 24.0f, 0.01f, 1.0f),
                                                     0.0f, boostSliderValueToText, boostSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("type"), String("Type"), String(),
                                                     NormalisableRange<float>(0.0f, 2.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_12_equalizerAudioProcessor::Plugex_12_equalizerAudioProcessor()
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
    boostParameter = parameters.getRawParameterValue("boost");
    typeParameter = parameters.getRawParameterValue("type");
}

Plugex_12_equalizerAudioProcessor::~Plugex_12_equalizerAudioProcessor()
{
}

//==============================================================================
const String Plugex_12_equalizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_12_equalizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_12_equalizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_12_equalizerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_12_equalizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_12_equalizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_12_equalizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_12_equalizerAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_12_equalizerAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_12_equalizerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_12_equalizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
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
    boostSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    boostSmoothed.setCurrentAndTargetValue(*boostParameter);
}

void Plugex_12_equalizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_12_equalizerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Plugex_12_equalizerAudioProcessor::compute_coeffs_peak()
{
    float alphaMul = alpha * A;
    float alphaDiv = alpha / A;

    b0 = 1.0f + alphaMul;
    b1 = a1 = -2.0f * c;
    b2 = 1.0f - alphaMul;
    a0 = 1.0f / (1.0f + alphaDiv);
    a2 = 1.0f - alphaDiv;
}

void Plugex_12_equalizerAudioProcessor::compute_coeffs_lowshelf()
{
    float twoSqrtAAlpha = sqrtf(A * 2.0f) * alpha;
    float AminOneC = (A - 1.0f) * c;
    float AAddOneC = (A + 1.0f) * c;

    b0 = A * ((A + 1.0f) - AminOneC + twoSqrtAAlpha);
    b1 = 2.0f * A * ((A - 1.0f) - AAddOneC);
    b2 = A * ((A + 1.0f) - AminOneC - twoSqrtAAlpha);
    a0 = 1.0f / ((A + 1.0f) + AminOneC + twoSqrtAAlpha);
    a1 = -2.0f * ((A - 1.0f) + AAddOneC);
    a2 = (A + 1.0f) + AminOneC - twoSqrtAAlpha;
}

void Plugex_12_equalizerAudioProcessor::compute_coeffs_highshelf()
{
    float twoSqrtAAlpha = sqrtf(A * 2.0f) * alpha;
    float AminOneC = (A - 1.0f) * c;
    float AAddOneC = (A + 1.0f) * c;

    b0 = A * ((A + 1.0f) + AminOneC + twoSqrtAAlpha);
    b1 = -2.0f * A * ((A - 1.0f) + AAddOneC);
    b2 = A * ((A + 1.0f) + AminOneC - twoSqrtAAlpha);
    a0 = 1.0f / ((A + 1.0f) - AminOneC + twoSqrtAAlpha);
    a1 = 2.0f * ((A - 1.0f) - AAddOneC);
    a2 = (A + 1.0f) - AminOneC - twoSqrtAAlpha;
}

void Plugex_12_equalizerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    freqSmoothed.setTargetValue(*freqParameter);
    qSmoothed.setTargetValue(*qParameter);
    boostSmoothed.setTargetValue(*boostParameter);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float cf = freqSmoothed.getNextValue();
        float q = qSmoothed.getNextValue();
        float boo = boostSmoothed.getNextValue();

        A = powf(10.0f, boo / 40.0f);
        w0 = cf * 2.0f * M_PI / currentSampleRate;
        c = cosf(w0);
        alpha = sinf(w0) / (2.0f * q);

        int type = (int) *typeParameter;
        switch (type) {
            case 0:
                compute_coeffs_peak(); break;
            case 1:
                compute_coeffs_lowshelf(); break;
            case 2:
                compute_coeffs_highshelf(); break;
        }

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer (channel);
            float filtered = ( b0 * channelData[i] + b1 * lastInputSample1[channel] + b2 * lastInputSample2[channel] - 
                             a1 * lastFilteredSample1[channel] - a2 * lastFilteredSample2[channel] ) * a0;
            lastInputSample2[channel] = lastInputSample1[channel];
            lastInputSample1[channel] = channelData[i];
            lastFilteredSample2[channel] = lastFilteredSample1[channel];
            channelData[i] = lastFilteredSample1[channel] = filtered;
        }
    }
}

//==============================================================================
bool Plugex_12_equalizerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_12_equalizerAudioProcessor::createEditor()
{
    return new Plugex_12_equalizerAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_12_equalizerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_12_equalizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_12_equalizerAudioProcessor();
}
