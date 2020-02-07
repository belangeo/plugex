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

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Plugex_34_granularStretcherAudioProcessorEditor::Plugex_34_granularStretcherAudioProcessorEditor (Plugex_34_granularStretcherAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 220);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("lightblue");

    title.setText("Plugex - 34 - Granular Stretcher", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    activeButton.setLookAndFeel(&plugexLookAndFeel);
    activeButton.setButtonText("Active");
    activeButton.setClickingTogglesState(true);
    addAndMakeVisible(&activeButton);

    activeAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "active", activeButton));

    durationLabel.setText("Duration", NotificationType::dontSendNotification);
    durationLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&durationLabel);

    durationKnob.setLookAndFeel(&plugexLookAndFeel);
    durationKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    durationKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&durationKnob);

    durationAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "duration", durationKnob));

    pitchLabel.setText("Pitch", NotificationType::dontSendNotification);
    pitchLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&pitchLabel);

    pitchKnob.setLookAndFeel(&plugexLookAndFeel);
    pitchKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    pitchKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&pitchKnob);

    pitchAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "pitch", pitchKnob));

    speedLabel.setText("Speed", NotificationType::dontSendNotification);
    speedLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&speedLabel);

    speedKnob.setLookAndFeel(&plugexLookAndFeel);
    speedKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    speedKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&speedKnob);

    speedAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "speed", speedKnob));

    jitterLabel.setText("Jitter", NotificationType::dontSendNotification);
    jitterLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&jitterLabel);

    jitterKnob.setLookAndFeel(&plugexLookAndFeel);
    jitterKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    jitterKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&jitterKnob);

    jitterAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "jitter", jitterKnob));
}

Plugex_34_granularStretcherAudioProcessorEditor::~Plugex_34_granularStretcherAudioProcessorEditor()
{
    activeButton.setLookAndFeel(nullptr);
    durationKnob.setLookAndFeel(nullptr);
    pitchKnob.setLookAndFeel(nullptr);
    speedKnob.setLookAndFeel(nullptr);
    jitterKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_34_granularStretcherAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_34_granularStretcherAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    activeButton.setBounds(area.removeFromTop(24));
    area.removeFromTop(12);

    auto area2 = area.removeFromTop(100);

    auto durationArea = area2.removeFromLeft(width/4.0f).withSizeKeepingCentre(80, 100);
    durationLabel.setBounds(durationArea.removeFromTop(20));
    durationKnob.setBounds(durationArea);

    auto pitchArea = area2.removeFromLeft(width/4.0f).withSizeKeepingCentre(80, 100);
    pitchLabel.setBounds(pitchArea.removeFromTop(20));
    pitchKnob.setBounds(pitchArea);

    auto speedArea = area2.removeFromLeft(width/4.0f).withSizeKeepingCentre(80, 100);
    speedLabel.setBounds(speedArea.removeFromTop(20));
    speedKnob.setBounds(speedArea);

    auto jitterArea = area2.withSizeKeepingCentre(80, 100);
    jitterLabel.setBounds(jitterArea.removeFromTop(20));
    jitterKnob.setBounds(jitterArea);

    area.removeFromTop(12);
}
