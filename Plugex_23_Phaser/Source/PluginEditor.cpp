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
Plugex_23_phaserAudioProcessorEditor::Plugex_23_phaserAudioProcessorEditor (Plugex_23_phaserAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("orange");

    title.setText("Plugex - 23 - Phaser", NotificationType::dontSendNotification);
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

    spreadLabel.setText("Spread", NotificationType::dontSendNotification);
    spreadLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&spreadLabel);

    spreadKnob.setLookAndFeel(&plugexLookAndFeel);
    spreadKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    spreadKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&spreadKnob);

    spreadAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "spread", spreadKnob));

    qLabel.setText("Q", NotificationType::dontSendNotification);
    qLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&qLabel);

    qKnob.setLookAndFeel(&plugexLookAndFeel);
    qKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    qKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&qKnob);

    qAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "q", qKnob));

    feedbackLabel.setText("Feedback", NotificationType::dontSendNotification);
    feedbackLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&feedbackLabel);

    feedbackKnob.setLookAndFeel(&plugexLookAndFeel);
    feedbackKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    feedbackKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&feedbackKnob);

    feedbackAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "feedback", feedbackKnob));
}

Plugex_23_phaserAudioProcessorEditor::~Plugex_23_phaserAudioProcessorEditor()
{
    freqKnob.setLookAndFeel(nullptr);
    spreadKnob.setLookAndFeel(nullptr);
    qKnob.setLookAndFeel(nullptr);
    feedbackKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_23_phaserAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_23_phaserAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto area1 = area.removeFromTop(100);
    auto freqArea = area1.removeFromLeft(width/4.0f).withSizeKeepingCentre(80, 100);
    freqLabel.setBounds(freqArea.removeFromTop(20));
    freqKnob.setBounds(freqArea);

    auto spreadArea = area1.removeFromLeft(width/4.0f).withSizeKeepingCentre(80, 100);
    spreadLabel.setBounds(spreadArea.removeFromTop(20));
    spreadKnob.setBounds(spreadArea);

    auto qArea = area1.removeFromLeft(width/4.0f).withSizeKeepingCentre(80, 100);
    qLabel.setBounds(qArea.removeFromTop(20));
    qKnob.setBounds(qArea);

    auto feedbackArea = area1.withSizeKeepingCentre(80, 100);
    feedbackLabel.setBounds(feedbackArea.removeFromTop(20));
    feedbackKnob.setBounds(feedbackArea);

    area.removeFromTop(12);
}
