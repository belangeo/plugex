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
#include "PluginProcessor.h"
#include "PlugexLookAndFeel.h"

//==============================================================================
/**
*/
class Plugex_41_resamplerAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_41_resamplerAudioProcessorEditor (Plugex_41_resamplerAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_41_resamplerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Plugex_41_resamplerAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;

    Label  srscaleLabel;
    Slider srscaleKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> srscaleAttachment;

    Label  bitdepthLabel;
    Slider bitdepthKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> bitdepthAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_41_resamplerAudioProcessorEditor)
};
