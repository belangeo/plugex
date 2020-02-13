/*******************************************************************************
* Plugex - PLUGin EXamples
*
* Plugex est une série de plugiciels auto-documentés permettant une étude 
* autonome du développement de plugiciels avec JUCE ainsi que des bases du
* traitement de signal audio avec le langage C++.
*
* © Olivier Bélanger 2020
*
*******************************************************************************/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Plugex_36_sineWaveMidiSynthAudioProcessorEditor::Plugex_36_sineWaveMidiSynthAudioProcessorEditor (Plugex_36_sineWaveMidiSynthAudioProcessor& p,
                                                                                                  AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts),
      keyboardComponent (p.keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    setSize (500, 280);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("lightgreen");

    title.setText("Plugex - 36 - Sine Wave Midi Synth", NotificationType::dontSendNotification);
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

    addAndMakeVisible(keyboardComponent);
}

Plugex_36_sineWaveMidiSynthAudioProcessorEditor::~Plugex_36_sineWaveMidiSynthAudioProcessorEditor()
{
}

//==============================================================================
void Plugex_36_sineWaveMidiSynthAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_36_sineWaveMidiSynthAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto gainArea = area.removeFromTop(100).withSizeKeepingCentre(80, 100);
    gainLabel.setBounds(gainArea.removeFromTop(20));
    gainKnob.setBounds(gainArea);
    area.removeFromTop(12);

    keyboardComponent.setBounds(area.removeFromBottom(80));
}
