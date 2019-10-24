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
Plugex_14_rectifierAudioProcessorEditor::Plugex_14_rectifierAudioProcessorEditor (Plugex_14_rectifierAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("red");

    title.setText("Plugex - 14 - Wave Rectifier", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    percentLabel.setText("Percentage", NotificationType::dontSendNotification);
    percentLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&percentLabel);

    percentKnob.setLookAndFeel(&plugexLookAndFeel);
    percentKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    percentKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&percentKnob);

    percentAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "percent", percentKnob));

    cutoffLabel.setText("Cutoff", NotificationType::dontSendNotification);
    cutoffLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&cutoffLabel);

    cutoffKnob.setLookAndFeel(&plugexLookAndFeel);
    cutoffKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    cutoffKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&cutoffKnob);

    cutoffAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "cutoff", cutoffKnob));
}

Plugex_14_rectifierAudioProcessorEditor::~Plugex_14_rectifierAudioProcessorEditor()
{
    percentKnob.setLookAndFeel(nullptr);
    cutoffKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_14_rectifierAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_14_rectifierAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto area2 = area.removeFromTop(100);
    auto percentArea = area2.removeFromLeft(width/2.0f).withSizeKeepingCentre(80, 100);
    percentLabel.setBounds(percentArea.removeFromTop(20));
    percentKnob.setBounds(percentArea);

    auto cutoffArea = area2.withSizeKeepingCentre(80, 100);
    cutoffLabel.setBounds(cutoffArea.removeFromTop(20));
    cutoffKnob.setBounds(cutoffArea);

    area.removeFromTop(12);
}
