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
class Plugex_23_phaserAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_23_phaserAudioProcessorEditor (Plugex_23_phaserAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_23_phaserAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Plugex_23_phaserAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;

    Label  freqLabel;
    Slider freqKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> freqAttachment;

    Label  spreadLabel;
    Slider spreadKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> spreadAttachment;

    Label  qLabel;
    Slider qKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> qAttachment;

    Label  feedbackLabel;
    Slider feedbackKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_23_phaserAudioProcessorEditor)
};
