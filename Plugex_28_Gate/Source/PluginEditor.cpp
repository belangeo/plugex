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
Plugex_28_gateAudioProcessorEditor::Plugex_28_gateAudioProcessorEditor (Plugex_28_gateAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("grey");

    title.setText("Plugex - 28 - Noise Gate", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    threshLabel.setText("Threshold", NotificationType::dontSendNotification);
    threshLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&threshLabel);

    threshKnob.setLookAndFeel(&plugexLookAndFeel);
    threshKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    threshKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&threshKnob);

    threshAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "thresh", threshKnob));

    risetimeLabel.setText("Rise TIme", NotificationType::dontSendNotification);
    risetimeLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&risetimeLabel);

    risetimeKnob.setLookAndFeel(&plugexLookAndFeel);
    risetimeKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    risetimeKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&risetimeKnob);

    risetimeAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "risetime", risetimeKnob));

    falltimeLabel.setText("Fall Time", NotificationType::dontSendNotification);
    falltimeLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&falltimeLabel);

    falltimeKnob.setLookAndFeel(&plugexLookAndFeel);
    falltimeKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    falltimeKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&falltimeKnob);

    falltimeAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "falltime", falltimeKnob));

    lookaheadLabel.setText("Look Ahead", NotificationType::dontSendNotification);
    lookaheadLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&lookaheadLabel);

    lookaheadKnob.setLookAndFeel(&plugexLookAndFeel);
    lookaheadKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    lookaheadKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&lookaheadKnob);

    lookaheadAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "lookahead", lookaheadKnob));
}

Plugex_28_gateAudioProcessorEditor::~Plugex_28_gateAudioProcessorEditor()
{
    threshKnob.setLookAndFeel(nullptr);
    risetimeKnob.setLookAndFeel(nullptr);
    falltimeKnob.setLookAndFeel(nullptr);
    lookaheadKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_28_gateAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_28_gateAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto area1 = area.removeFromTop(100);
    auto threshArea = area1.removeFromLeft(width/4.0f).withSizeKeepingCentre(80, 100);
    threshLabel.setBounds(threshArea.removeFromTop(20));
    threshKnob.setBounds(threshArea);

    auto risetimeArea = area1.removeFromLeft(width/4.0f).withSizeKeepingCentre(80, 100);
    risetimeLabel.setBounds(risetimeArea.removeFromTop(20));
    risetimeKnob.setBounds(risetimeArea);

    auto falltimeArea = area1.removeFromLeft(width/4.0f).withSizeKeepingCentre(80, 100);
    falltimeLabel.setBounds(falltimeArea.removeFromTop(20));
    falltimeKnob.setBounds(falltimeArea);

    auto lookaheadArea = area1.withSizeKeepingCentre(80, 100);
    lookaheadLabel.setBounds(lookaheadArea.removeFromTop(20));
    lookaheadKnob.setBounds(lookaheadArea);

    area.removeFromTop(12);
}
