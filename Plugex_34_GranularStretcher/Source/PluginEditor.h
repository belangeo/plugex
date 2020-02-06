/*******************************************************************************
* Plugex - PLUGin EXamples
*
* Plugex est une série de plugiciels auto-documentés permettant une étude 
* autonome du développement de plugiciels avec JUCE ainsi que des bases du
* traitement de signal audio avec le langage C++.
*
* © Olivier Bélanger 2020
*
*******************************************************************************/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "PlugexLookAndFeel.h"

//==============================================================================
/**
*/
class Plugex_34_granularStretcherAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_34_granularStretcherAudioProcessorEditor (Plugex_34_granularStretcherAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_34_granularStretcherAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Plugex_34_granularStretcherAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;

    TextButton activeButton;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> activeAttachment;

    Label  durationLabel;
    Slider durationKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> durationAttachment;

    Label  pitchLabel;
    Slider pitchKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> pitchAttachment;

    Label  speedLabel;
    Slider speedKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> speedAttachment;

    Label  jitterLabel;
    Slider jitterKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> jitterAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_34_granularStretcherAudioProcessorEditor)
};
