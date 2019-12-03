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
Plugex_30_expanderAudioProcessorEditor::Plugex_30_expanderAudioProcessorEditor (Plugex_30_expanderAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("grey");

    title.setText("Plugex - 30 - Expander", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    downthreshLabel.setText("Down Thresh", NotificationType::dontSendNotification);
    downthreshLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&downthreshLabel);

    downthreshKnob.setLookAndFeel(&plugexLookAndFeel);
    downthreshKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    downthreshKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&downthreshKnob);

    downthreshAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "downthresh", downthreshKnob));

    upthreshLabel.setText("Up Thresh", NotificationType::dontSendNotification);
    upthreshLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&upthreshLabel);

    upthreshKnob.setLookAndFeel(&plugexLookAndFeel);
    upthreshKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    upthreshKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&upthreshKnob);

    upthreshAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "upthresh", upthreshKnob));

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

Plugex_30_expanderAudioProcessorEditor::~Plugex_30_expanderAudioProcessorEditor()
{
    downthreshKnob.setLookAndFeel(nullptr);
    upthreshKnob.setLookAndFeel(nullptr);
    ratioKnob.setLookAndFeel(nullptr);
    risetimeKnob.setLookAndFeel(nullptr);
    falltimeKnob.setLookAndFeel(nullptr);
    lookaheadKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_30_expanderAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_30_expanderAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto areaTop = area.removeFromTop(120);
    auto downthreshArea = areaTop.removeFromLeft(width/3.0f).withSizeKeepingCentre(80, 100);
    downthreshLabel.setBounds(downthreshArea.removeFromTop(20));
    downthreshKnob.setBounds(downthreshArea);

    auto upthreshArea = areaTop.removeFromLeft(width/3.0f).withSizeKeepingCentre(80, 100);
    upthreshLabel.setBounds(upthreshArea.removeFromTop(20));
    upthreshKnob.setBounds(upthreshArea);

    auto ratioArea = areaTop.withSizeKeepingCentre(80, 100);
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
