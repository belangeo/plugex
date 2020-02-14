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
#include "DelayLine.h"
#include "OnePoleLowpass.h"

//==============================================================================
/**
*/
class Plugex_28_gateAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Plugex_28_gateAudioProcessor();
    ~Plugex_28_gateAudioProcessor();

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

    std::atomic<float> *threshParameter = nullptr;
    SmoothedValue<float> threshSmoothed;

    std::atomic<float> *risetimeParameter = nullptr;
    SmoothedValue<float> risetimeSmoothed;

    std::atomic<float> *falltimeParameter = nullptr;
    SmoothedValue<float> falltimeSmoothed;

    std::atomic<float> *lookaheadParameter = nullptr;
    SmoothedValue<float> lookaheadSmoothed;

    OnePoleLowpass lowpassFilter[2];
    OnePoleLowpass gateFilter[2];
    DelayLine      lookaheadDelay[2];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_28_gateAudioProcessor)
};
