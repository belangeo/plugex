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
class Plugex_10_butterworthBrAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_10_butterworthBrAudioProcessorEditor (Plugex_10_butterworthBrAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_10_butterworthBrAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Plugex_10_butterworthBrAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;

    Label  freqLabel;
    Slider freqKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> freqAttachment;

    Label  qLabel;
    Slider qKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> qAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_10_butterworthBrAudioProcessorEditor)
};
