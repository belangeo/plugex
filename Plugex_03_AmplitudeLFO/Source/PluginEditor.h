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

class Plugex_03_amplitudeLfoAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_03_amplitudeLfoAudioProcessorEditor (Plugex_03_amplitudeLfoAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_03_amplitudeLfoAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    Plugex_03_amplitudeLfoAudioProcessor& processor;

    PlugexLookAndFeel plugexLookAndFeel;

    AudioProcessorValueTreeState& valueTreeState;

    Label title;

    Label  freqLabel;
    Slider freqKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> freqAttachment;

    Label  depthLabel;
    Slider depthKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> depthAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_03_amplitudeLfoAudioProcessorEditor)
};
