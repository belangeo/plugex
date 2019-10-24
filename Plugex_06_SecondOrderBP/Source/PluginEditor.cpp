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
Plugex_06_secondOrderBpAudioProcessorEditor::Plugex_06_secondOrderBpAudioProcessorEditor (Plugex_06_secondOrderBpAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("green");

    title.setText("Plugex - 06 - Second Order Bandpass Filter", NotificationType::dontSendNotification);
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

    qLabel.setText("Resonance", NotificationType::dontSendNotification);
    qLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&qLabel);

    qKnob.setLookAndFeel(&plugexLookAndFeel);
    qKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    qKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&qKnob);

    qAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "q", qKnob));
}

Plugex_06_secondOrderBpAudioProcessorEditor::~Plugex_06_secondOrderBpAudioProcessorEditor()
{
    freqKnob.setLookAndFeel(nullptr);
    qKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_06_secondOrderBpAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_06_secondOrderBpAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto area2 = area.removeFromTop(100);
    auto freqArea = area2.removeFromLeft(width/2.0f).withSizeKeepingCentre(80, 100);
    freqLabel.setBounds(freqArea.removeFromTop(20));
    freqKnob.setBounds(freqArea);

    auto qArea = area2.withSizeKeepingCentre(80, 100);
    qLabel.setBounds(qArea.removeFromTop(20));
    qKnob.setBounds(qArea);

    area.removeFromTop(12);
}
