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
Plugex_12_equalizerAudioProcessorEditor::Plugex_12_equalizerAudioProcessorEditor (Plugex_12_equalizerAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("green");

    title.setText("Plugex - 12 - Equalizer", NotificationType::dontSendNotification);
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

    qLabel.setText("Q", NotificationType::dontSendNotification);
    qLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&qLabel);

    qKnob.setLookAndFeel(&plugexLookAndFeel);
    qKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    qKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&qKnob);

    qAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "q", qKnob));

    boostLabel.setText("Boost", NotificationType::dontSendNotification);
    boostLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&boostLabel);

    boostKnob.setLookAndFeel(&plugexLookAndFeel);
    boostKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    boostKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&boostKnob);

    boostAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "boost", boostKnob));

    typeLabel.setText("Filter Type", NotificationType::dontSendNotification);
    typeLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&typeLabel);

    typeCombo.setLookAndFeel(&plugexLookAndFeel);
    typeCombo.addItemList({"Peak/Notch", "Lowshelf", "Highshelf"}, 1);
    typeCombo.setSelectedId(1);
    addAndMakeVisible(&typeCombo);

    typeAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "type", typeCombo));
}

Plugex_12_equalizerAudioProcessorEditor::~Plugex_12_equalizerAudioProcessorEditor()
{
    freqKnob.setLookAndFeel(nullptr);
    qKnob.setLookAndFeel(nullptr);
    boostKnob.setLookAndFeel(nullptr);
    typeCombo.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_12_equalizerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_12_equalizerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto area2 = area.removeFromTop(100);

    auto freqArea = area2.removeFromLeft(width/4.44f).withSizeKeepingCentre(80, 100);
    freqLabel.setBounds(freqArea.removeFromTop(20));
    freqKnob.setBounds(freqArea);

    auto qArea = area2.removeFromLeft(width/4.44f).withSizeKeepingCentre(80, 100);
    qLabel.setBounds(qArea.removeFromTop(20));
    qKnob.setBounds(qArea);

    auto boostArea = area2.removeFromLeft(width/4.44f).withSizeKeepingCentre(80, 100);
    boostLabel.setBounds(boostArea.removeFromTop(20));
    boostKnob.setBounds(boostArea);

    auto typeArea = area2.withSizeKeepingCentre(120, 100);
    typeLabel.setBounds(typeArea.removeFromTop(20));
    typeCombo.setBounds(typeArea.removeFromTop(60).withSizeKeepingCentre(120, 24));

    area.removeFromTop(12);
}
