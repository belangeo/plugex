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
Plugex_24_pannerAudioProcessorEditor::Plugex_24_pannerAudioProcessorEditor (Plugex_24_pannerAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("purple");

    title.setText("Plugex - 24 - Panner", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    panLabel.setText("Pan", NotificationType::dontSendNotification);
    panLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&panLabel);

    panKnob.setLookAndFeel(&plugexLookAndFeel);
    panKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    panKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&panKnob);

    panAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "pan", panKnob));

    typeLabel.setText("Algorithm", NotificationType::dontSendNotification);
    typeLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&typeLabel);

    typeCombo.setLookAndFeel(&plugexLookAndFeel);
    typeCombo.addItemList({"Linear", "Square Root", "Sine / Cosine"}, 1);
    typeCombo.setSelectedId(1);
    addAndMakeVisible(&typeCombo);

    typeAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "type", typeCombo));
}

Plugex_24_pannerAudioProcessorEditor::~Plugex_24_pannerAudioProcessorEditor()
{
    panKnob.setLookAndFeel(nullptr);
    typeCombo.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_24_pannerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_24_pannerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto area2 = area.removeFromTop(100);
    auto panArea = area2.removeFromLeft(width/2.f).withSizeKeepingCentre(80, 100);
    panLabel.setBounds(panArea.removeFromTop(20));
    panKnob.setBounds(panArea);

    auto typeArea = area2.withSizeKeepingCentre(120, 100);
    typeLabel.setBounds(typeArea.removeFromTop(20));
    typeCombo.setBounds(typeArea.removeFromTop(60).withSizeKeepingCentre(120, 24));

    area.removeFromTop(12);
}
