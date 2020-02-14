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

class Plugex_00_templateFftAudioProcessor  : public AudioProcessor,
                                             public FFTEngine::Listener
{
public:
    //==============================================================================
    Plugex_00_templateFftAudioProcessor();
    ~Plugex_00_templateFftAudioProcessor();

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

    // FFTEngine::Listener
    void fftEngineFrameReady(FFTEngine *engine, float *fftData, int fftSize) override;

private:
    //==============================================================================
    AudioProcessorValueTreeState parameters;

    FFTEngine fftEngine[2];

    int lastOrder;
    int lastOverlaps;
    int lastWintype;

    std::atomic<float> *orderParameter = nullptr;
    std::atomic<float> *overlapsParameter = nullptr;
    std::atomic<float> *wintypeParameter = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_00_templateFftAudioProcessor)
};
