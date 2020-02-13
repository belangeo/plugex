/*******************************************************************************
* Plugex - PLUGin EXamples
*
* Plugex est une série de plugiciels auto-documentés permettant une étude 
* autonome du développement de plugiciels avec JUCE ainsi que des bases du
* traitement de signal audio avec le langage C++.
*
* © Olivier Bélanger 2020
*
*******************************************************************************/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "PlugexLookAndFeel.h"

class Plugex_36_sineWaveMidiSynthAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_36_sineWaveMidiSynthAudioProcessorEditor (Plugex_36_sineWaveMidiSynthAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_36_sineWaveMidiSynthAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    Plugex_36_sineWaveMidiSynthAudioProcessor& processor;

    PlugexLookAndFeel plugexLookAndFeel;

    AudioProcessorValueTreeState& valueTreeState;

    Label title;

    Label  gainLabel;
    Slider gainKnob;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    MidiKeyboardComponent keyboardComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_36_sineWaveMidiSynthAudioProcessorEditor)
};
