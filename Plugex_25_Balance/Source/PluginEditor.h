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

class Plugex_25_balanceAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_25_balanceAudioProcessorEditor (Plugex_25_balanceAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_25_balanceAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    Plugex_25_balanceAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;

    Label  balLabel;
    Slider balKnob;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> balAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_25_balanceAudioProcessorEditor)
};
