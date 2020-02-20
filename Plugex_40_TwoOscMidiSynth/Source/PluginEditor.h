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

class Plugex_40_twoOscMidiSynthAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_40_twoOscMidiSynthAudioProcessorEditor (Plugex_40_twoOscMidiSynthAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_40_twoOscMidiSynthAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    Plugex_40_twoOscMidiSynthAudioProcessor& processor;

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
    Label lfo1typeLabel;
    ComboBox lfo1typeCombo;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> lfo1typeAttachment;

    Label  lfo1freqLabel;
    Slider lfo1freqKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> lfo1freqAttachment;

    Label  lfo1depthLabel;
    Slider lfo1depthKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> lfo1depthAttachment;

    ToggleButton lfo1RouteLfo2Freq;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteLfo2FreqAttachment;
    ToggleButton lfo1RouteLfo2FreqInv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteLfo2FreqInvAttachment;
    ToggleButton lfo1RouteLfo2Depth;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteLfo2DepthAttachment;
    ToggleButton lfo1RouteLfo2DepthInv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteLfo2DepthInvAttachment;

    ToggleButton lfo1RouteFreq1;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteFreq1Attachment;
    ToggleButton lfo1RouteFreq1Inv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteFreq1InvAttachment;
    ToggleButton lfo1RouteSharp1;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteSharp1Attachment;
    ToggleButton lfo1RouteSharp1Inv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteSharp1InvAttachment;
    ToggleButton lfo1RouteGain1;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteGain1Attachment;
    ToggleButton lfo1RouteGain1Inv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteGain1InvAttachment;

    ToggleButton lfo1RouteFreq2;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteFreq2Attachment;
    ToggleButton lfo1RouteFreq2Inv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteFreq2InvAttachment;
    ToggleButton lfo1RouteSharp2;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteSharp2Attachment;
    ToggleButton lfo1RouteSharp2Inv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteSharp2InvAttachment;
    ToggleButton lfo1RouteGain2;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteGain2Attachment;
    ToggleButton lfo1RouteGain2Inv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo1RouteGain2InvAttachment;

    //---------------------------------------------------------------------------------
    Label lfo2typeLabel;
    ComboBox lfo2typeCombo;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> lfo2typeAttachment;

    Label  lfo2freqLabel;
    Slider lfo2freqKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> lfo2freqAttachment;

    Label  lfo2depthLabel;
    Slider lfo2depthKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> lfo2depthAttachment;

    ToggleButton lfo2RouteFreq1;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo2RouteFreq1Attachment;
    ToggleButton lfo2RouteFreq1Inv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo2RouteFreq1InvAttachment;
    ToggleButton lfo2RouteSharp1;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo2RouteSharp1Attachment;
    ToggleButton lfo2RouteSharp1Inv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo2RouteSharp1InvAttachment;
    ToggleButton lfo2RouteGain1;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo2RouteGain1Attachment;
    ToggleButton lfo2RouteGain1Inv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo2RouteGain1InvAttachment;

    ToggleButton lfo2RouteFreq2;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo2RouteFreq2Attachment;
    ToggleButton lfo2RouteFreq2Inv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo2RouteFreq2InvAttachment;
    ToggleButton lfo2RouteSharp2;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo2RouteSharp2Attachment;
    ToggleButton lfo2RouteSharp2Inv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo2RouteSharp2InvAttachment;
    ToggleButton lfo2RouteGain2;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo2RouteGain2Attachment;
    ToggleButton lfo2RouteGain2Inv;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> lfo2RouteGain2InvAttachment;

    //---------------------------------------------------------------------------------
    Label type1Label;
    ComboBox type1Combo;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> type1Attachment;

    Label  sharp1Label;
    Slider sharp1Knob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sharp1Attachment;

    Label  gain1Label;
    Slider gain1Knob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gain1Attachment;

    ToggleButton stereo1Toggle;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> stereo1ToggleAttachment;

    //---------------------------------------------------------------------------------
    Label type2Label;
    ComboBox type2Combo;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> type2Attachment;

    Label  sharp2Label;
    Slider sharp2Knob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sharp2Attachment;

    Label  gain2Label;
    Slider gain2Knob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gain2Attachment;

    ToggleButton stereo2Toggle;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> stereo2ToggleAttachment;

    MidiKeyboardComponent keyboardComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_40_twoOscMidiSynthAudioProcessorEditor)
};
