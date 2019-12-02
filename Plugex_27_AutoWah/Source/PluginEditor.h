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
class Plugex_27_autoWahAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_27_autoWahAudioProcessorEditor (Plugex_27_autoWahAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_27_autoWahAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Plugex_27_autoWahAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;

    Label  responseLabel;
    Slider responseKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> responseAttachment;

    Label  maxFreqLabel;
    Slider maxFreqKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> maxFreqAttachment;

    Label  qLabel;
    Slider qKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> qAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_27_autoWahAudioProcessorEditor)
};
