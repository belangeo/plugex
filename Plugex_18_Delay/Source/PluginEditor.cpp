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
Plugex_18_delayAudioProcessorEditor::Plugex_18_delayAudioProcessorEditor (Plugex_18_delayAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("orange");

    title.setText("Plugex - 18 - Delay Line", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    timeLabel.setText("Delay Time", NotificationType::dontSendNotification);
    timeLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&timeLabel);

    timeKnob.setLookAndFeel(&plugexLookAndFeel);
    timeKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    timeKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&timeKnob);

    timeAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "time", timeKnob));

    feedbackLabel.setText("Feedback", NotificationType::dontSendNotification);
    feedbackLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&feedbackLabel);

    feedbackKnob.setLookAndFeel(&plugexLookAndFeel);
    feedbackKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    feedbackKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&feedbackKnob);

    feedbackAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "feedback", feedbackKnob));

    balanceLabel.setText("Balance", NotificationType::dontSendNotification);
    balanceLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&balanceLabel);

    balanceKnob.setLookAndFeel(&plugexLookAndFeel);
    balanceKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    balanceKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&balanceKnob);

    balanceAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "balance", balanceKnob));
}

Plugex_18_delayAudioProcessorEditor::~Plugex_18_delayAudioProcessorEditor()
{
    timeKnob.setLookAndFeel(nullptr);
    feedbackKnob.setLookAndFeel(nullptr);
    balanceKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_18_delayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_18_delayAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto area2 = area.removeFromTop(100);
    auto timeArea = area2.removeFromLeft(width/3.0f).withSizeKeepingCentre(80, 100);
    timeLabel.setBounds(timeArea.removeFromTop(20));
    timeKnob.setBounds(timeArea);

    auto feedbackArea = area2.removeFromLeft(width/3.0f).withSizeKeepingCentre(80, 100);
    feedbackLabel.setBounds(feedbackArea.removeFromTop(20));
    feedbackKnob.setBounds(feedbackArea);

    auto balanceArea = area2.withSizeKeepingCentre(80, 100);
    balanceLabel.setBounds(balanceArea.removeFromTop(20));
    balanceKnob.setBounds(balanceArea);

    area.removeFromTop(12);
}
