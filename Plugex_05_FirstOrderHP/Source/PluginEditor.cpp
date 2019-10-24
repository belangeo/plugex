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

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Plugex_05_firstOrderHpAudioProcessorEditor::Plugex_05_firstOrderHpAudioProcessorEditor (Plugex_05_firstOrderHpAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("green");

    title.setText("Plugex - 05 - First Order Highpass Filter", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    freqLabel.setText("Freq", NotificationType::dontSendNotification);
    freqLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&freqLabel);

    freqKnob.setLookAndFeel(&plugexLookAndFeel);
    freqKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    freqKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&freqKnob);

    freqAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "freq", freqKnob));

}

Plugex_05_firstOrderHpAudioProcessorEditor::~Plugex_05_firstOrderHpAudioProcessorEditor()
{
    freqKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_05_firstOrderHpAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_05_firstOrderHpAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto freqArea = area.removeFromTop(100).withSizeKeepingCentre(80, 100);
    freqLabel.setBounds(freqArea.removeFromTop(20));
    freqKnob.setBounds(freqArea);
    area.removeFromTop(12);
}
