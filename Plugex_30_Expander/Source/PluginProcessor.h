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

//==============================================================================
/**
*/
class Plugex_30_expanderAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Plugex_30_expanderAudioProcessor();
    ~Plugex_30_expanderAudioProcessor();

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
    float follower[2];

    float *downthreshParameter = nullptr;
    SmoothedValue<float> downthreshSmoothed;

    float *upthreshParameter = nullptr;
    SmoothedValue<float> upthreshSmoothed;

    float *ratioParameter = nullptr;
    SmoothedValue<float> ratioSmoothed;

    float *risetimeParameter = nullptr;
    SmoothedValue<float> risetimeSmoothed;

    float *falltimeParameter = nullptr;
    SmoothedValue<float> falltimeSmoothed;

    float *lookaheadParameter = nullptr;
    SmoothedValue<float> lookaheadSmoothed;

    DelayLine lookaheadDelay[2];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_30_expanderAudioProcessor)
};
