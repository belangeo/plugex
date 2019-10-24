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
class Plugex_11_biquadAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_11_biquadAudioProcessorEditor (Plugex_11_biquadAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_11_biquadAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Plugex_11_biquadAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;

    Label  freqLabel;
    Slider freqKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> freqAttachment;

    Label  qLabel;
    Slider qKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> qAttachment;

    Label typeLabel;
    ComboBox typeCombo;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> typeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_11_biquadAudioProcessorEditor)
};
