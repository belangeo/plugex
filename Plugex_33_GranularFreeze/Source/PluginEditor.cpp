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
Plugex_33_granularFreezeAudioProcessorEditor::Plugex_33_granularFreezeAudioProcessorEditor (Plugex_33_granularFreezeAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 220);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("lightblue");

    title.setText("Plugex - 33 - Granulator Freeze", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    activeButton.setLookAndFeel(&plugexLookAndFeel);
    activeButton.setButtonText("Active");
    activeButton.setClickingTogglesState(true);
    addAndMakeVisible(&activeButton);

    activeAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "active", activeButton));

    densityLabel.setText("Density", NotificationType::dontSendNotification);
    densityLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&densityLabel);

    densityKnob.setLookAndFeel(&plugexLookAndFeel);
    densityKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    densityKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&densityKnob);

    densityAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "density", densityKnob));

    pitchLabel.setText("Pitch", NotificationType::dontSendNotification);
    pitchLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&pitchLabel);

    pitchKnob.setLookAndFeel(&plugexLookAndFeel);
    pitchKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    pitchKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&pitchKnob);

    pitchAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "pitch", pitchKnob));

    durationLabel.setText("Gr. Dur", NotificationType::dontSendNotification);
    durationLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&durationLabel);

    durationKnob.setLookAndFeel(&plugexLookAndFeel);
    durationKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    durationKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&durationKnob);

    durationAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "duration", durationKnob));

    jitterLabel.setText("Jitter", NotificationType::dontSendNotification);
    jitterLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&jitterLabel);

    jitterKnob.setLookAndFeel(&plugexLookAndFeel);
    jitterKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    jitterKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&jitterKnob);

    jitterAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "jitter", jitterKnob));
}

Plugex_33_granularFreezeAudioProcessorEditor::~Plugex_33_granularFreezeAudioProcessorEditor()
{
    activeButton.setLookAndFeel(nullptr);
    densityKnob.setLookAndFeel(nullptr);
    pitchKnob.setLookAndFeel(nullptr);
    durationKnob.setLookAndFeel(nullptr);
    jitterKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_33_granularFreezeAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_33_granularFreezeAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    activeButton.setBounds(area.removeFromTop(24));
    area.removeFromTop(12);

    auto area2 = area.removeFromTop(100);

    auto densityArea = area2.removeFromLeft(width/4.0f).withSizeKeepingCentre(80, 100);
    densityLabel.setBounds(densityArea.removeFromTop(20));
    densityKnob.setBounds(densityArea);

    auto pitchArea = area2.removeFromLeft(width/4.0f).withSizeKeepingCentre(80, 100);
    pitchLabel.setBounds(pitchArea.removeFromTop(20));
    pitchKnob.setBounds(pitchArea);

    auto durationArea = area2.removeFromLeft(width/4.0f).withSizeKeepingCentre(80, 100);
    durationLabel.setBounds(durationArea.removeFromTop(20));
    durationKnob.setBounds(durationArea);

    auto jitterArea = area2.withSizeKeepingCentre(80, 100);
    jitterLabel.setBounds(jitterArea.removeFromTop(20));
    jitterKnob.setBounds(jitterArea);

    area.removeFromTop(12);
}
