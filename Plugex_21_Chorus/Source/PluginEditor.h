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
class Plugex_21_chorusAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_21_chorusAudioProcessorEditor (Plugex_21_chorusAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_21_chorusAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Plugex_21_chorusAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;

    Label  depthLabel;
    Slider depthKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> depthAttachment;

    Label  feedbackLabel;
    Slider feedbackKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;

    Label  balanceLabel;
    Slider balanceKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> balanceAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_21_chorusAudioProcessorEditor)
};
