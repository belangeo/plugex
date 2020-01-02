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
Plugex_41_resamplerAudioProcessorEditor::Plugex_41_resamplerAudioProcessorEditor (Plugex_41_resamplerAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("pink");

    title.setText("Plugex - 41 - Resampler", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    srscaleLabel.setText("SR Scale", NotificationType::dontSendNotification);
    srscaleLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&srscaleLabel);

    srscaleKnob.setLookAndFeel(&plugexLookAndFeel);
    srscaleKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    srscaleKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&srscaleKnob);

    srscaleAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "srscale", srscaleKnob));

    bitdepthLabel.setText("Bit Depth", NotificationType::dontSendNotification);
    bitdepthLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&bitdepthLabel);

    bitdepthKnob.setLookAndFeel(&plugexLookAndFeel);
    bitdepthKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    bitdepthKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&bitdepthKnob);

    bitdepthAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "bitdepth", bitdepthKnob));
}

Plugex_41_resamplerAudioProcessorEditor::~Plugex_41_resamplerAudioProcessorEditor()
{
    srscaleKnob.setLookAndFeel(nullptr);
    bitdepthKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_41_resamplerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_41_resamplerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto area2 = area.removeFromTop(100);
    auto srscaleArea = area2.removeFromLeft(width/2.0f).withSizeKeepingCentre(80, 100);
    srscaleLabel.setBounds(srscaleArea.removeFromTop(20));
    srscaleKnob.setBounds(srscaleArea);

    auto bitdepthArea = area2.withSizeKeepingCentre(80, 100);
    bitdepthLabel.setBounds(bitdepthArea.removeFromTop(20));
    bitdepthKnob.setBounds(bitdepthArea);

    area.removeFromTop(12);
}
