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

//==============================================================================
/**
*/
class Plugex_35_granularSoundcloudAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_35_granularSoundcloudAudioProcessorEditor (Plugex_35_granularSoundcloudAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_35_granularSoundcloudAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Plugex_35_granularSoundcloudAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;

    TextButton activeButton;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> activeAttachment;

    Label  densityLabel;
    Slider densityKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> densityAttachment;

    Label  rndpitLabel;
    Slider rndpitKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> rndpitAttachment;

    Label  rndposLabel;
    Slider rndposKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> rndposAttachment;

    Label  rnddurLabel;
    Slider rnddurKnob;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> rnddurAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_35_granularSoundcloudAudioProcessorEditor)
};
