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
#include "MultiSlider.h"

class Plugex_32_spectralDelayAudioProcessorEditor  : public AudioProcessorEditor,
                                                 public MultiSlider::Listener,
                                                 public Timer
{
public:
    Plugex_32_spectralDelayAudioProcessorEditor (Plugex_32_spectralDelayAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_32_spectralDelayAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

    void timerCallback() override;
    void multiSliderChanged(MultiSlider *multiSlider, const Array<float> &value) override;

private:
    Plugex_32_spectralDelayAudioProcessor& processor;

    PlugexLookAndFeel plugexLookAndFeel;

    AudioProcessorValueTreeState& valueTreeState;

    Label title;

    Label orderLabel;
    ComboBox orderCombo;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> orderAttachment;

    Label overlapsLabel;
    ComboBox overlapsCombo;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> overlapsAttachment;

    Label wintypeLabel;
    ComboBox wintypeCombo;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> wintypeAttachment;

    Label  delayLabel;
    MultiSlider delayMultiSlider;

    Label  feedbackLabel;
    MultiSlider feedbackMultiSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_32_spectralDelayAudioProcessorEditor)
};
