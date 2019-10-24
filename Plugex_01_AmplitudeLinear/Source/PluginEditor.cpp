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
Plugex_01_amplitudeLinearAudioProcessorEditor::Plugex_01_amplitudeLinearAudioProcessorEditor (Plugex_01_amplitudeLinearAudioProcessor& p,
                                                                                              AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("blue");

    title.setText("Plugex - 01 - Amplitude Linear", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    gainLabel.setText("Gain", NotificationType::dontSendNotification);
    gainLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&gainLabel);

    gainKnob.setLookAndFeel(&plugexLookAndFeel);
    gainKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    gainKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&gainKnob);

    gainAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "gain", gainKnob));

}

Plugex_01_amplitudeLinearAudioProcessorEditor::~Plugex_01_amplitudeLinearAudioProcessorEditor()
{
}

//==============================================================================
void Plugex_01_amplitudeLinearAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_01_amplitudeLinearAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto gainArea = area.removeFromTop(100).withSizeKeepingCentre(80, 100);
    gainLabel.setBounds(gainArea.removeFromTop(20));
    gainKnob.setBounds(gainArea);
    area.removeFromTop(12);
}
