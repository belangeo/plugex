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
Plugex_25_balanceAudioProcessorEditor::Plugex_25_balanceAudioProcessorEditor (Plugex_25_balanceAudioProcessor& p,
                                                                                              AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("purple");

    title.setText("Plugex - 25 - Stereo Balance", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    balLabel.setText("Balance", NotificationType::dontSendNotification);
    balLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&balLabel);

    balKnob.setLookAndFeel(&plugexLookAndFeel);
    balKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    balKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&balKnob);

    balAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "bal", balKnob));

}

Plugex_25_balanceAudioProcessorEditor::~Plugex_25_balanceAudioProcessorEditor()
{
    balKnob.setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_25_balanceAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_25_balanceAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto balArea = area.removeFromTop(100).withSizeKeepingCentre(80, 100);
    balLabel.setBounds(balArea.removeFromTop(20));
    balKnob.setBounds(balArea);
    area.removeFromTop(12);
}
