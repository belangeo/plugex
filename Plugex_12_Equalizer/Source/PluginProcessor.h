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

//==============================================================================
/**
*/
class Plugex_12_equalizerAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Plugex_12_equalizerAudioProcessor();
    ~Plugex_12_equalizerAudioProcessor();

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
    float lastInputSample1[8];
    float lastInputSample2[8];
    float lastFilteredSample1[8];
    float lastFilteredSample2[8];

    // Filter variables.
    float A;
    float w0;
    float c;
    float alpha;

    // Filter coefficients.
    float a0;
    float a1;
    float a2;
    float b0;
    float b1;
    float b2;

    // Filter coefficients computing functions.
    void compute_coeffs_peak();
    void compute_coeffs_lowshelf();
    void compute_coeffs_highshelf();

    float *freqParameter = nullptr;
    SmoothedValue<float> freqSmoothed;

    float *qParameter = nullptr;
    SmoothedValue<float> qSmoothed;

    float *boostParameter = nullptr;
    SmoothedValue<float> boostSmoothed;

    float *typeParameter = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_12_equalizerAudioProcessor)
};
