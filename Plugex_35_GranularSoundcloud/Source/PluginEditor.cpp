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
Plugex_35_granularSoundcloudAudioProcessorEditor::Plugex_35_granularSoundcloudAudioProcessorEditor (Plugex_35_granularSoundcloudAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 220);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("lightblue");

    title.setText("Plugex - 35 - Granular Soundcloud", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    activeButton.setLookAndFeel(&plugexLookAndFeel);
    activeButton.setButtonText("Active");
    activeButton.setClickingTogglesState(true);
    addAndMakeVisible(&activeButton);

    activeAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "active", activeButton));

    densityLabel.setText("Density", NotificationType::dontSendNotification);
    densityLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&densityLabel);

    densityKnob.setLookAndFeel(&plugexLookAndFeel);
    densityKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    densityKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&densityKnob);

    densityAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "density", densityKnob));

    rndpitLabel.setText("Rnd Pit", NotificationType::dontSendNotification);
    rndpitLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&rndpitLabel);

    rndpitKnob.setLookAndFeel(&plugexLookAndFeel);
    rndpitKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    rndpitKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&rndpitKnob);

    rndpitAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "rndpit", rndpitKnob));

    rndposLabel.setText("Rnd Pos", NotificationType::dontSendNotification);
    rndposLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&rndposLabel);

    rndposKnob.setLookAndFeel(&plugexLookAndFeel);
    rndposKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    rndposKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&rndposKnob);

    rndposAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "rndpos", rndposKnob));

    rnddurLabel.setText("Rnd Dur", NotificationType::dontSendNotification);
    rnddurLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&rnddurLabel);

    rnddurKnob.setLookAndFeel(&plugexLookAndFeel);
    rnddurKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    rnddurKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&rnddurKnob);

    rnddurAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "rnddur", rnddurKnob));
}

Plugex_35_granularSoundcloudAudioProcessorEditor::~Plugex_35_granularSoundcloudAudioProcessorEditor()
{
    activeButton.setLookAndFeel(nullptr);
    densityKnob.setLookAndFeel(nullptr);
    rndpitKnob.setLookAndFeel(nullptr);
    rndposKnob.setLookAndFeel(nullptr);
    rnddurKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_35_granularSoundcloudAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_35_granularSoundcloudAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    activeButton.setBounds(area.removeFromTop(24));
    area.removeFromTop(12);

    auto area2 = area.removeFromTop(100);

    auto densityArea = area2.removeFromLeft(width/4.0f).withSizeKeepingCentre(80, 100);
    densityLabel.setBounds(densityArea.removeFromTop(20));
    densityKnob.setBounds(densityArea);

    auto rndpitArea = area2.removeFromLeft(width/4.0f).withSizeKeepingCentre(80, 100);
    rndpitLabel.setBounds(rndpitArea.removeFromTop(20));
    rndpitKnob.setBounds(rndpitArea);

    auto rndposArea = area2.removeFromLeft(width/4.0f).withSizeKeepingCentre(80, 100);
    rndposLabel.setBounds(rndposArea.removeFromTop(20));
    rndposKnob.setBounds(rndposArea);

    auto rnddurArea = area2.withSizeKeepingCentre(80, 100);
    rnddurLabel.setBounds(rnddurArea.removeFromTop(20));
    rnddurKnob.setBounds(rnddurArea);

    area.removeFromTop(12);
}
