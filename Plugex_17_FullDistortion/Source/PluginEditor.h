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
class Plugex_17_fullDistortionAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_17_fullDistortionAudioProcessorEditor (Plugex_17_fullDistortionAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_17_fullDistortionAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Plugex_17_fullDistortionAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;

    Label  highpassFreqLabel;
    Slider highpassFreqKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> highpassFreqAttachment;

    Label  highpassQLabel;
    Slider highpassQKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> highpassQAttachment;

    Label  driveLabel;
    Slider driveKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> driveAttachment;

    Label  lowpassFreqLabel;
    Slider lowpassFreqKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> lowpassFreqAttachment;

    Label  lowpassQLabel;
    Slider lowpassQKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> lowpassQAttachment;

    Label  balanceLabel;
    Slider balanceKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> balanceAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_17_fullDistortionAudioProcessorEditor)
};
