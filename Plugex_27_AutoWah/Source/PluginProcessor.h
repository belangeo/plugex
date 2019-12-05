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

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "OnePoleLowpass.h"
#include "Biquad.h"

//==============================================================================
/**
*/
class Plugex_27_autoWahAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Plugex_27_autoWahAudioProcessor();
    ~Plugex_27_autoWahAudioProcessor();

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

private:
    //==============================================================================
    AudioProcessorValueTreeState parameters;

    double currentSampleRate;

    float *responseParameter = nullptr;
    SmoothedValue<float> responseSmoothed;

    float *maxFreqParameter = nullptr;
    SmoothedValue<float> maxFreqSmoothed;

    float *qParameter = nullptr;
    SmoothedValue<float> qSmoothed;

    OnePoleLowpass lowpassFilter[2];
    Biquad bandpassFilter[2];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_27_autoWahAudioProcessor)
};