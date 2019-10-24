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

class Plugex_05_firstOrderHpAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_05_firstOrderHpAudioProcessorEditor (Plugex_05_firstOrderHpAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_05_firstOrderHpAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    Plugex_05_firstOrderHpAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;

    Label  freqLabel;
    Slider freqKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> freqAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_05_firstOrderHpAudioProcessorEditor)
};
