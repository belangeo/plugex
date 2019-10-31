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
#include "SinOsc.h"

//==============================================================================
/**
*/
class Plugex_21_chorusAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Plugex_21_chorusAudioProcessor();
    ~Plugex_21_chorusAudioProcessor();

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

    const float centerDelayTimes[8] = {0.0087, 0.0102, 0.011, 0.0125, 0.0134, 0.015, 0.0171, 0.0178};
    const float delayTimeDevs[8] = {0.001, 0.0012, 0.0013, 0.0014, 0.0015, 0.0016, 0.002, 0.0023};
    const float lfoFrequencies[8] = {1.879, 1.654, 1.342, 1.231, 0.879, 0.657, 0.465, 0.254};
    
    double currentSampleRate;

    float *depthParameter = nullptr;
    SmoothedValue<float> depthSmoothed;

    float *feedbackParameter = nullptr;
    SmoothedValue<float> feedbackSmoothed;

    float *balanceParameter = nullptr;
    SmoothedValue<float> balanceSmoothed;

    SinOsc lfoDelayTime[8][2];
    DelayLine delayLine[8][2];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_21_chorusAudioProcessor)
};
