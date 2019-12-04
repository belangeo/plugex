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

class Plugex_00_templateFftAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_00_templateFftAudioProcessorEditor (Plugex_00_templateFftAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~Plugex_00_templateFftAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    Plugex_00_templateFftAudioProcessor& processor;

    PlugexLookAndFeel plugexLookAndFeel;

    AudioProcessorValueTreeState& valueTreeState;

    Label title;
    Label  description;

    Label orderLabel;
    ComboBox orderCombo;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> orderAttachment;

    Label overlapsLabel;
    ComboBox overlapsCombo;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> overlapsAttachment;

    Label wintypeLabel;
    ComboBox wintypeCombo;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> wintypeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_00_templateFftAudioProcessorEditor)
};
