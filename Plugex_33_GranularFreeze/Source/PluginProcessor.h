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

#include "Granulator.h"

//==============================================================================
/**
*/
class Plugex_33_granularFreezeAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Plugex_33_granularFreezeAudioProcessor();
    ~Plugex_33_granularFreezeAudioProcessor();

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

    Random jitterRandom;

    Granulator granulator[2];

    float portLastSample = 0.f;

    bool isActive = false;
    std::atomic<float> *activeParameter = nullptr;

    std::atomic<float> *densityParameter = nullptr;
    SmoothedValue<float> densitySmoothed;

    std::atomic<float> *pitchParameter = nullptr;
    SmoothedValue<float> pitchSmoothed;

    std::atomic<float> *durationParameter = nullptr;
    SmoothedValue<float> durationSmoothed;

    std::atomic<float> *jitterParameter = nullptr;
    SmoothedValue<float> jitterSmoothed;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_33_granularFreezeAudioProcessor)
};
