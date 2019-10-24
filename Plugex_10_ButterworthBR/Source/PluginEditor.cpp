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
Plugex_10_butterworthBrAudioProcessorEditor::Plugex_10_butterworthBrAudioProcessorEditor (Plugex_10_butterworthBrAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("green");

    title.setText("Plugex - 10 - Butterworth Bandstop", NotificationType::dontSendNotification);
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

Plugex_10_butterworthBrAudioProcessorEditor::~Plugex_10_butterworthBrAudioProcessorEditor()
{
    freqKnob.setLookAndFeel(nullptr);
    qKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_10_butterworthBrAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_10_butterworthBrAudioProcessorEditor::resized()
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
