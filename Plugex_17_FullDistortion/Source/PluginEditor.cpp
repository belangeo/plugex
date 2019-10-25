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
Plugex_17_fullDistortionAudioProcessorEditor::Plugex_17_fullDistortionAudioProcessorEditor (Plugex_17_fullDistortionAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("red");

    title.setText("Plugex - 17 - Full Distortion", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    highpassFreqLabel.setText("Highpass Freq", NotificationType::dontSendNotification);
    highpassFreqLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&highpassFreqLabel);

    highpassFreqKnob.setLookAndFeel(&plugexLookAndFeel);
    highpassFreqKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    highpassFreqKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&highpassFreqKnob);

    highpassFreqAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "highpassFreq", highpassFreqKnob));

    highpassQLabel.setText("Highpass Q", NotificationType::dontSendNotification);
    highpassQLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&highpassQLabel);

    highpassQKnob.setLookAndFeel(&plugexLookAndFeel);
    highpassQKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    highpassQKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&highpassQKnob);

    highpassQAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "highpassQ", highpassQKnob));

    driveLabel.setText("Drive", NotificationType::dontSendNotification);
    driveLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&driveLabel);

    driveKnob.setLookAndFeel(&plugexLookAndFeel);
    driveKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    driveKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&driveKnob);

    driveAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "drive", driveKnob));

    lowpassFreqLabel.setText("Lowpass Freq", NotificationType::dontSendNotification);
    lowpassFreqLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&lowpassFreqLabel);

    lowpassFreqKnob.setLookAndFeel(&plugexLookAndFeel);
    lowpassFreqKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    lowpassFreqKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&lowpassFreqKnob);

    lowpassFreqAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "lowpassFreq", lowpassFreqKnob));

    lowpassQLabel.setText("Lowpass Q", NotificationType::dontSendNotification);
    lowpassQLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&lowpassQLabel);

    lowpassQKnob.setLookAndFeel(&plugexLookAndFeel);
    lowpassQKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    lowpassQKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&lowpassQKnob);

    lowpassQAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "lowpassQ", lowpassQKnob));

    balanceLabel.setText("Balance", NotificationType::dontSendNotification);
    balanceLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&balanceLabel);

    balanceKnob.setLookAndFeel(&plugexLookAndFeel);
    balanceKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    balanceKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&balanceKnob);

    balanceAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "balance", balanceKnob));
}

Plugex_17_fullDistortionAudioProcessorEditor::~Plugex_17_fullDistortionAudioProcessorEditor()
{
    highpassFreqKnob.setLookAndFeel(nullptr);
    highpassQKnob.setLookAndFeel(nullptr);
    driveKnob.setLookAndFeel(nullptr);
    lowpassFreqKnob.setLookAndFeel(nullptr);
    lowpassQKnob.setLookAndFeel(nullptr);
    balanceKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_17_fullDistortionAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_17_fullDistortionAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto areaTop = area.removeFromTop(120);
    auto highpassFreqArea = areaTop.removeFromLeft(width/3.0f).withSizeKeepingCentre(100, 100);
    highpassFreqLabel.setBounds(highpassFreqArea.removeFromTop(20));
    highpassFreqKnob.setBounds(highpassFreqArea);

    auto highpassQArea = areaTop.removeFromLeft(width/3.0f).withSizeKeepingCentre(80, 100);
    highpassQLabel.setBounds(highpassQArea.removeFromTop(20));
    highpassQKnob.setBounds(highpassQArea);

    auto driveArea = areaTop.withSizeKeepingCentre(80, 100);
    driveLabel.setBounds(driveArea.removeFromTop(20));
    driveKnob.setBounds(driveArea);

    auto areaBottom = area.removeFromTop(120);
    auto lowpassFreqArea = areaBottom.removeFromLeft(width/3.0f).withSizeKeepingCentre(100, 100);
    lowpassFreqLabel.setBounds(lowpassFreqArea.removeFromTop(20));
    lowpassFreqKnob.setBounds(lowpassFreqArea);

    auto lowpassQArea = areaBottom.removeFromLeft(width/3.0f).withSizeKeepingCentre(80, 100);
    lowpassQLabel.setBounds(lowpassQArea.removeFromTop(20));
    lowpassQKnob.setBounds(lowpassQArea);

    auto balanceArea = areaBottom.withSizeKeepingCentre(80, 100);
    balanceLabel.setBounds(balanceArea.removeFromTop(20));
    balanceKnob.setBounds(balanceArea);
}
