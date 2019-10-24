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

class Plugex_01_amplitudeLinearAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_01_amplitudeLinearAudioProcessorEditor (Plugex_01_amplitudeLinearAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_01_amplitudeLinearAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    Plugex_01_amplitudeLinearAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;

    Label  gainLabel;
    Slider gainKnob;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_01_amplitudeLinearAudioProcessorEditor)
};
