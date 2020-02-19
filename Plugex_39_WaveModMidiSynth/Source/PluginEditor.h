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

class Plugex_39_waveModMidiSynthAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_39_waveModMidiSynthAudioProcessorEditor (Plugex_39_waveModMidiSynthAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_39_waveModMidiSynthAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    Plugex_39_waveModMidiSynthAudioProcessor& processor;

    PlugexLookAndFeel plugexLookAndFeel;

    AudioProcessorValueTreeState& valueTreeState;

    Label title;

    Label  attackLabel;
    Slider attackKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> attackAttachment;

    Label  decayLabel;
    Slider decayKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> decayAttachment;

    Label  sustainLabel;
    Slider sustainKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;

    Label  releaseLabel;
    Slider releaseKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;

    //---------------------------------------------------------------------------------
    Label lfotypeLabel;
    ComboBox lfotypeCombo;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> lfotypeAttachment;

    Label  lfofreqLabel;
    Slider lfofreqKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> lfofreqAttachment;

    Label  lfodepthLabel;
    Slider lfodepthKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> lfodepthAttachment;

    ToggleButton lfoRouteFreq;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfoRouteFreqAttachment;
    ToggleButton lfoRouteFreqInv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfoRouteFreqInvAttachment;
    ToggleButton lfoRouteSharp;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfoRouteSharpAttachment;
    ToggleButton lfoRouteSharpInv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfoRouteSharpInvAttachment;
    ToggleButton lfoRouteGain;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfoRouteGainAttachment;
    ToggleButton lfoRouteGainInv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfoRouteGainInvAttachment;

    //---------------------------------------------------------------------------------
    Label typeLabel;
    ComboBox typeCombo;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> typeAttachment;

    Label  sharpLabel;
    Slider sharpKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sharpAttachment;

    Label  gainLabel;
    Slider gainKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    ToggleButton stereoToggle;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> stereoToggleAttachment;

    MidiKeyboardComponent keyboardComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_39_waveModMidiSynthAudioProcessorEditor)
};
