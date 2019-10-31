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
Plugex_20_flangerAudioProcessorEditor::Plugex_20_flangerAudioProcessorEditor (Plugex_20_flangerAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("orange");

    title.setText("Plugex - 20 - Flanger", NotificationType::dontSendNotification);
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

    delayLabel.setText("Central Delay", NotificationType::dontSendNotification);
    delayLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&delayLabel);

    delayKnob.setLookAndFeel(&plugexLookAndFeel);
    delayKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    delayKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&delayKnob);

    delayAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "delay", delayKnob));

    depthLabel.setText("Depth", NotificationType::dontSendNotification);
    depthLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&depthLabel);

    depthKnob.setLookAndFeel(&plugexLookAndFeel);
    depthKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    depthKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&depthKnob);

    depthAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "depth", depthKnob));
}

Plugex_20_flangerAudioProcessorEditor::~Plugex_20_flangerAudioProcessorEditor()
{
    freqKnob.setLookAndFeel(nullptr);
    delayKnob.setLookAndFeel(nullptr);
    depthKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_20_flangerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_20_flangerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto area2 = area.removeFromTop(100);
    auto freqArea = area2.removeFromLeft(width/3.0f).withSizeKeepingCentre(80, 100);
    freqLabel.setBounds(freqArea.removeFromTop(20));
    freqKnob.setBounds(freqArea);

    auto delayArea = area2.removeFromLeft(width/3.0f).withSizeKeepingCentre(80, 100);
    delayLabel.setBounds(delayArea.removeFromTop(20));
    delayKnob.setBounds(delayArea);

    auto depthArea = area2.withSizeKeepingCentre(80, 100);
    depthLabel.setBounds(depthArea.removeFromTop(20));
    depthKnob.setBounds(depthArea);

    area.removeFromTop(12);
}
