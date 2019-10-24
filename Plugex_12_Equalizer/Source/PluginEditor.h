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
class Plugex_12_equalizerAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_12_equalizerAudioProcessorEditor (Plugex_12_equalizerAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_12_equalizerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Plugex_12_equalizerAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;

    Label  freqLabel;
    Slider freqKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> freqAttachment;

    Label  qLabel;
    Slider qKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> qAttachment;

    Label  boostLabel;
    Slider boostKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> boostAttachment;

    Label typeLabel;
    ComboBox typeCombo;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> typeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_12_equalizerAudioProcessorEditor)
};
