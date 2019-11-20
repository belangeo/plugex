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
Plugex_26_dopplerAudioProcessorEditor::Plugex_26_dopplerAudioProcessorEditor (Plugex_26_dopplerAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("purple");

    title.setText("Plugex - 26 - Ping-pong Doppler", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    freqLabel.setText("Frequency", NotificationType::dontSendNotification);
    freqLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&freqLabel);

    freqKnob.setLookAndFeel(&plugexLookAndFeel);
    freqKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    freqKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&freqKnob);

    freqAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "freq", freqKnob));

    depthLabel.setText("Depth", NotificationType::dontSendNotification);
    depthLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&depthLabel);

    depthKnob.setLookAndFeel(&plugexLookAndFeel);
    depthKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    depthKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&depthKnob);

    depthAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "depth", depthKnob));
}

Plugex_26_dopplerAudioProcessorEditor::~Plugex_26_dopplerAudioProcessorEditor()
{
    freqKnob.setLookAndFeel(nullptr);
    depthKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_26_dopplerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_26_dopplerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto area2 = area.removeFromTop(100);
    auto freqArea = area2.removeFromLeft(width/2.0f).withSizeKeepingCentre(80, 100);
    freqLabel.setBounds(freqArea.removeFromTop(20));
    freqKnob.setBounds(freqArea);

    auto depthArea = area2.withSizeKeepingCentre(80, 100);
    depthLabel.setBounds(depthArea.removeFromTop(20));
    depthKnob.setBounds(depthArea);

    area.removeFromTop(12);
}
