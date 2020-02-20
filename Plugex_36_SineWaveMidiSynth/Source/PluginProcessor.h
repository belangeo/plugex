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
#include "SinOsc.h"

//==============================================================================
struct SineWaveSound   : public SynthesiserSound
{
    SineWaveSound() {}
    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }
};

//==============================================================================
struct SineWaveVoice   : public SynthesiserVoice
{
    SineWaveVoice();

    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}

    bool canPlaySound (SynthesiserSound *sound) override;

    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound *, int /*currentPitchWheelPosition*/) override;
    void stopNote (float /*velocity*/, bool allowTailOff) override;

    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

private:
    SinOsc oscillator;
    double level = 0.0, tailOff = 0.0;
};

//==============================================================================
class Plugex_36_sineWaveMidiSynthAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Plugex_36_sineWaveMidiSynthAudioProcessor();
    ~Plugex_36_sineWaveMidiSynthAudioProcessor();

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

    Synthesiser synthesiser;
    
    std::atomic<float> *gainParameter = nullptr;
    float lastGain = 0.f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_36_sineWaveMidiSynthAudioProcessor)
};
