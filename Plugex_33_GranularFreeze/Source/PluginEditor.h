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
class Plugex_33_granularFreezeAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_33_granularFreezeAudioProcessorEditor (Plugex_33_granularFreezeAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_33_granularFreezeAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Plugex_33_granularFreezeAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;

    TextButton activeButton;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> activeAttachment;

    Label  densityLabel;
    Slider densityKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> densityAttachment;

    Label  pitchLabel;
    Slider pitchKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> pitchAttachment;

    Label  durationLabel;
    Slider durationKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> durationAttachment;

    Label  jitterLabel;
    Slider jitterKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> jitterAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_33_granularFreezeAudioProcessorEditor)
};
