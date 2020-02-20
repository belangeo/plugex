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

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "BandLimitedOsc.h"

//==============================================================================
struct MySynthesiserSound   : public SynthesiserSound
{
    MySynthesiserSound() {}
    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }
};

//==============================================================================
struct MySynthesiserVoice   : public SynthesiserVoice
{
    MySynthesiserVoice();

    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}

    bool canPlaySound (SynthesiserSound *sound) override;

    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound *, int /*currentPitchWheelPosition*/) override;
    void stopNote (float /*velocity*/, bool allowTailOff) override;

    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

    void setEnvelopeParameters(ADSR::Parameters params);
    void setWavetypeParameter(int type1, int type2);
    void setSharpParameter(float sharp1, float sharp2);
    void setGainParameter(float gain1, float gain2);
    void setStereoToggleParameter(int state1, int state2);
    void setLFOWavetypeParameter(int type1, int type2);
    void setLFOFreqParameter(float freq1, float freq2);
    void setLFODepthParameter(float depth1, float depth2);
    void setLFO1RoutingParameters(int lfo2freq, int lfo2freqInv, int lfo2depth, int lfo2depthInv,
                                  int freq1, int freq1Inv, int sharp1, int sharp1Inv, int gain1, int gain1Inv,
                                  int freq2, int freq2Inv, int sharp2, int sharp2Inv, int gain2, int gain2Inv);
    void setLFO2RoutingParameters(int freq1, int freq1Inv, int sharp1, int sharp1Inv, int gain1, int gain1Inv,
                                  int freq2, int freq2Inv, int sharp2, int sharp2Inv, int gain2, int gain2Inv);

private:
    ADSR envelope;
    BandLimitedOsc lfo1;
    BandLimitedOsc lfo2;
    BandLimitedOsc oscillator1Left;
    BandLimitedOsc oscillator1Right;
    BandLimitedOsc oscillator2Left;
    BandLimitedOsc oscillator2Right;
    int stereoSwitch1 = 0, stereoSwitch2 = 0;
    int lfo1lfo2freqSwith = 0, lfo1lfo2freqSwithInv = 0, lfo1lfo2depthSwith = 0, lfo1lfo2depthSwithInv = 0;
    int lfo1freqSwitch1 = 0, lfo1freqSwitch1Inv = 0, lfo1sharpSwitch1 = 0, lfo1sharpSwitch1Inv = 0, lfo1gainSwitch1 = 0, lfo1gainSwitch1Inv = 0;
    int lfo1freqSwitch2 = 0, lfo1freqSwitch2Inv = 0, lfo1sharpSwitch2 = 0, lfo1sharpSwitch2Inv = 0, lfo1gainSwitch2 = 0, lfo1gainSwitch2Inv = 0;
    int lfo2freqSwitch1 = 0, lfo2freqSwitch1Inv = 0, lfo2sharpSwitch1 = 0, lfo2sharpSwitch1Inv = 0, lfo2gainSwitch1 = 0, lfo2gainSwitch1Inv = 0;
    int lfo2freqSwitch2 = 0, lfo2freqSwitch2Inv = 0, lfo2sharpSwitch2 = 0, lfo2sharpSwitch2Inv = 0, lfo2gainSwitch2 = 0, lfo2gainSwitch2Inv = 0;
    double noteSharp1 = 0.0, noteSharp2 = 0.0, lfoDepth1 = 0.0, lfoDepth2 = 0.0, lfoFreq1 = 0.0, lfoFreq2 = 0.0;
    double noteFreq = 0.0, level = 0.0;
    SmoothedValue<float> smoothedGain1, smoothedGain2;
};

//==============================================================================
class MySynthesiser : public Synthesiser
{
public:
    void setEnvelopeParameters(ADSR::Parameters params);
    void setWavetypeParameter(int type1, int type2);
    void setSharpParameter(float sharp1, float sharp2);
    void setGainParameter(float gain1, float gain2);
    void setStereoToggleParameter(int state1, int state2);
    void setLFOWavetypeParameter(int type1, int type2);
    void setLFOFreqParameter(float freq1, float freq2);
    void setLFODepthParameter(float depth1, float depth2);
    void setLFO1RoutingParameters(int lfo2freq, int lfo2freqInv, int lfo2depth, int lfo2depthInv,
                                  int freq1, int freq1Inv, int sharp1, int sharp1Inv, int gain1, int gain1Inv,
                                  int freq2, int freq2Inv, int sharp2, int sharp2Inv, int gain2, int gain2Inv);
    void setLFO2RoutingParameters(int freq1, int freq1Inv, int sharp1, int sharp1Inv, int gain1, int gain1Inv,
                                  int freq2, int freq2Inv, int sharp2, int sharp2Inv, int gain2, int gain2Inv);
};

//==============================================================================
class Plugex_40_twoOscMidiSynthAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Plugex_40_twoOscMidiSynthAudioProcessor();
    ~Plugex_40_twoOscMidiSynthAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    MidiKeyboardState keyboardState;

private:
    //==============================================================================
    AudioProcessorValueTreeState parameters;

    MySynthesiser synthesiser;

    std::atomic<float> *attackParameter = nullptr;
    std::atomic<float> *decayParameter = nullptr;
    std::atomic<float> *sustainParameter = nullptr;
    std::atomic<float> *releaseParameter = nullptr;

    std::atomic<float> *lfo1typeParameter = nullptr;
    std::atomic<float> *lfo1freqParameter = nullptr;
    std::atomic<float> *lfo1depthParameter = nullptr;

    std::atomic<float> *lfo1RouteLfo2FreqParameter = nullptr;
    std::atomic<float> *lfo1RouteLfo2FreqInvParameter = nullptr;
    std::atomic<float> *lfo1RouteLfo2DepthParameter = nullptr;
    std::atomic<float> *lfo1RouteLfo2DepthInvParameter = nullptr;
    std::atomic<float> *lfo1RouteFreq1Parameter = nullptr;
    std::atomic<float> *lfo1RouteFreq1InvParameter = nullptr;
    std::atomic<float> *lfo1RouteSharp1Parameter = nullptr;
    std::atomic<float> *lfo1RouteSharp1InvParameter = nullptr;
    std::atomic<float> *lfo1RouteGain1Parameter = nullptr;
    std::atomic<float> *lfo1RouteGain1InvParameter = nullptr;
    std::atomic<float> *lfo1RouteFreq2Parameter = nullptr;
    std::atomic<float> *lfo1RouteFreq2InvParameter = nullptr;
    std::atomic<float> *lfo1RouteSharp2Parameter = nullptr;
    std::atomic<float> *lfo1RouteSharp2InvParameter = nullptr;
    std::atomic<float> *lfo1RouteGain2Parameter = nullptr;
    std::atomic<float> *lfo1RouteGain2InvParameter = nullptr;

    std::atomic<float> *lfo2typeParameter = nullptr;
    std::atomic<float> *lfo2freqParameter = nullptr;
    std::atomic<float> *lfo2depthParameter = nullptr;

    std::atomic<float> *lfo2RouteFreq1Parameter = nullptr;
    std::atomic<float> *lfo2RouteFreq1InvParameter = nullptr;
    std::atomic<float> *lfo2RouteSharp1Parameter = nullptr;
    std::atomic<float> *lfo2RouteSharp1InvParameter = nullptr;
    std::atomic<float> *lfo2RouteGain1Parameter = nullptr;
    std::atomic<float> *lfo2RouteGain1InvParameter = nullptr;
    std::atomic<float> *lfo2RouteFreq2Parameter = nullptr;
    std::atomic<float> *lfo2RouteFreq2InvParameter = nullptr;
    std::atomic<float> *lfo2RouteSharp2Parameter = nullptr;
    std::atomic<float> *lfo2RouteSharp2InvParameter = nullptr;
    std::atomic<float> *lfo2RouteGain2Parameter = nullptr;
    std::atomic<float> *lfo2RouteGain2InvParameter = nullptr;

    std::atomic<float> *type1Parameter = nullptr;
    std::atomic<float> *sharp1Parameter = nullptr;
    std::atomic<float> *gain1Parameter = nullptr;
    std::atomic<float> *stereo1Parameter = nullptr;

    std::atomic<float> *type2Parameter = nullptr;
    std::atomic<float> *sharp2Parameter = nullptr;
    std::atomic<float> *gain2Parameter = nullptr;
    std::atomic<float> *stereo2Parameter = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_40_twoOscMidiSynthAudioProcessor)
};
