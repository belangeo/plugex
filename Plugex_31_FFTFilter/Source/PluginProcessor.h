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
#include "FFTEngine.h"

const int filterNumberOfPoints = 350;

class Plugex_31_fftFilterAudioProcessor  : public AudioProcessor,
                                             public FFTEngine::Listener
{
public:
    //==============================================================================
    Plugex_31_fftFilterAudioProcessor();
    ~Plugex_31_fftFilterAudioProcessor();

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

    void fftEngineFrameReady(float *fftData, int fftSize) override;

    void computeFFTFilter();
    void setFFTFilterPoints(const Array<float> &value);

    bool fftFilterPointsChanged = false;
    Array<float> fftFilterPoints;

private:
    //==============================================================================
    AudioProcessorValueTreeState parameters;

    FFTEngine fftEngine[2];
    float fftFilter[8193];

    int lastOrder;
    int lastOverlaps;
    int lastWintype;

    float *orderParameter = nullptr;
    float *overlapsParameter = nullptr;
    float *wintypeParameter = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_31_fftFilterAudioProcessor)
};
