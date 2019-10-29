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
class Plugex_18_delayAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_18_delayAudioProcessorEditor (Plugex_18_delayAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_18_delayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Plugex_18_delayAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;

    Label  timeLabel;
    Slider timeKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> timeAttachment;

    Label  feedbackLabel;
    Slider feedbackKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;

    Label  balanceLabel;
    Slider balanceKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> balanceAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_18_delayAudioProcessorEditor)
};
