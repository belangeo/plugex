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
Plugex_29_compressorAudioProcessorEditor::Plugex_29_compressorAudioProcessorEditor (Plugex_29_compressorAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("grey");

    title.setText("Plugex - 29 - Compressor", NotificationType::dontSendNotification);
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

    ratioLabel.setText("Ratio", NotificationType::dontSendNotification);
    ratioLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&ratioLabel);

    ratioKnob.setLookAndFeel(&plugexLookAndFeel);
    ratioKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    ratioKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&ratioKnob);

    ratioAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "ratio", ratioKnob));

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

Plugex_29_compressorAudioProcessorEditor::~Plugex_29_compressorAudioProcessorEditor()
{
    threshKnob.setLookAndFeel(nullptr);
    ratioKnob.setLookAndFeel(nullptr);
    risetimeKnob.setLookAndFeel(nullptr);
    falltimeKnob.setLookAndFeel(nullptr);
    lookaheadKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_29_compressorAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_29_compressorAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto areaTop = area.removeFromTop(120);
    auto threshArea = areaTop.removeFromLeft(width/2.0f).withSizeKeepingCentre(80, 100);
    threshLabel.setBounds(threshArea.removeFromTop(20));
    threshKnob.setBounds(threshArea);

    auto ratioArea = areaTop.removeFromLeft(width/2.0f).withSizeKeepingCentre(80, 100);
    ratioLabel.setBounds(ratioArea.removeFromTop(20));
    ratioKnob.setBounds(ratioArea);

    auto areaBottom = area.removeFromTop(120);
    auto risetimeArea = areaBottom.removeFromLeft(width/3.0f).withSizeKeepingCentre(80, 100);
    risetimeLabel.setBounds(risetimeArea.removeFromTop(20));
    risetimeKnob.setBounds(risetimeArea);

    auto falltimeArea = areaBottom.removeFromLeft(width/3.0f).withSizeKeepingCentre(80, 100);
    falltimeLabel.setBounds(falltimeArea.removeFromTop(20));
    falltimeKnob.setBounds(falltimeArea);

    auto lookaheadArea = areaBottom.withSizeKeepingCentre(80, 100);
    lookaheadLabel.setBounds(lookaheadArea.removeFromTop(20));
    lookaheadKnob.setBounds(lookaheadArea);

    area.removeFromTop(12);
}
