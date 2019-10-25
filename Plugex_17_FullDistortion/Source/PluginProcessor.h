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
#include "Biquad.h"

//==============================================================================
/**
*/
class Plugex_17_fullDistortionAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Plugex_17_fullDistortionAudioProcessor();
    ~Plugex_17_fullDistortionAudioProcessor();

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

    float *highpassFreqParameter = nullptr;
    SmoothedValue<float> highpassFreqSmoothed;

    float *highpassQParameter = nullptr;
    SmoothedValue<float> highpassQSmoothed;

    float *driveParameter = nullptr;
    SmoothedValue<float> driveSmoothed;

    float *lowpassFreqParameter = nullptr;
    SmoothedValue<float> lowpassFreqSmoothed;

    float *lowpassQParameter = nullptr;
    SmoothedValue<float> lowpassQSmoothed;

    float *balanceParameter = nullptr;
    SmoothedValue<float> balanceSmoothed;

    Biquad highpassFilter[8];
    Biquad lowpassFilter[8];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_17_fullDistortionAudioProcessor)
};
