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
Plugex_27_autoWahAudioProcessorEditor::Plugex_27_autoWahAudioProcessorEditor (Plugex_27_autoWahAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("grey");

    title.setText("Plugex - 27 - Auto-Wah", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    responseLabel.setText("Speediness", NotificationType::dontSendNotification);
    responseLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&responseLabel);

    responseKnob.setLookAndFeel(&plugexLookAndFeel);
    responseKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    responseKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&responseKnob);

    responseAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "response", responseKnob));

    maxFreqLabel.setText("Max Freq", NotificationType::dontSendNotification);
    maxFreqLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&maxFreqLabel);

    maxFreqKnob.setLookAndFeel(&plugexLookAndFeel);
    maxFreqKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    maxFreqKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&maxFreqKnob);

    maxFreqAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "maxFreq", maxFreqKnob));

    qLabel.setText("Filter Q", NotificationType::dontSendNotification);
    qLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&qLabel);

    qKnob.setLookAndFeel(&plugexLookAndFeel);
    qKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    qKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&qKnob);

    qAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "q", qKnob));
}

Plugex_27_autoWahAudioProcessorEditor::~Plugex_27_autoWahAudioProcessorEditor()
{
    responseKnob.setLookAndFeel(nullptr);
    maxFreqKnob.setLookAndFeel(nullptr);
    qKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_27_autoWahAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_27_autoWahAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto area2 = area.removeFromTop(100);
    auto responseArea = area2.removeFromLeft(width/3.0f).withSizeKeepingCentre(80, 100);
    responseLabel.setBounds(responseArea.removeFromTop(20));
    responseKnob.setBounds(responseArea);

    auto maxFreqArea = area2.removeFromLeft(width/3.0f).withSizeKeepingCentre(80, 100);
    maxFreqLabel.setBounds(maxFreqArea.removeFromTop(20));
    maxFreqKnob.setBounds(maxFreqArea);

    auto qArea = area2.withSizeKeepingCentre(80, 100);
    qLabel.setBounds(qArea.removeFromTop(20));
    qKnob.setBounds(qArea);

    area.removeFromTop(12);
}
