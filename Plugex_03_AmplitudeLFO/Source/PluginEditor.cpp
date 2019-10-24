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
Plugex_03_amplitudeLfoAudioProcessorEditor::Plugex_03_amplitudeLfoAudioProcessorEditor (Plugex_03_amplitudeLfoAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("blue");

    title.setText("Plugex - 03 - Oscillating Amplitude", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    freqLabel.setText("Mod Freq", NotificationType::dontSendNotification);
    freqLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&freqLabel);

    freqKnob.setLookAndFeel(&plugexLookAndFeel);
    freqKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    freqKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&freqKnob);

    freqAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "freq", freqKnob));

    depthLabel.setText("depth", NotificationType::dontSendNotification);
    depthLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&depthLabel);

    depthKnob.setLookAndFeel(&plugexLookAndFeel);
    depthKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    depthKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&depthKnob);

    depthAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "depth", depthKnob));
}

Plugex_03_amplitudeLfoAudioProcessorEditor::~Plugex_03_amplitudeLfoAudioProcessorEditor()
{
}

//==============================================================================
void Plugex_03_amplitudeLfoAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_03_amplitudeLfoAudioProcessorEditor::resized()
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
