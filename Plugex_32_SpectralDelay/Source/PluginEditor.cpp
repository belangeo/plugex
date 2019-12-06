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
Plugex_32_spectralDelayAudioProcessorEditor::Plugex_32_spectralDelayAudioProcessorEditor (Plugex_32_spectralDelayAudioProcessor& p,
                                                                                      AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    setSize (400, 450);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("steal");

    title.setText("Plugex - 32 - Spectral Delay", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    orderLabel.setText("FFT Size", NotificationType::dontSendNotification);
    orderLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&orderLabel);

    orderCombo.setLookAndFeel(&plugexLookAndFeel);
    orderCombo.addItemList({"64", "128", "256", "512", "1024", "2048", "4096", "8192", "16384"}, 6);
    orderCombo.setSelectedId(10);
    addAndMakeVisible(&orderCombo);

    orderAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "order", orderCombo));

    overlapsLabel.setText("FFT Overlaps", NotificationType::dontSendNotification);
    overlapsLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&overlapsLabel);

    overlapsCombo.setLookAndFeel(&plugexLookAndFeel);
    overlapsCombo.addItemList({"2", "4", "8"}, 1);
    overlapsCombo.setSelectedId(2);
    addAndMakeVisible(&overlapsCombo);

    overlapsAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "overlaps", overlapsCombo));

    wintypeLabel.setText("Window Type", NotificationType::dontSendNotification);
    wintypeLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&wintypeLabel);

    wintypeCombo.setLookAndFeel(&plugexLookAndFeel);
    wintypeCombo.addItemList({"Rectangular", "Triangular", "Hanning", "Hamming", "Blackman", "Blackman-Harris 4 terms",
                              "Blackman-Harris 7 terms", "Flat Top", "Half Sine"}, 1);
    wintypeCombo.setSelectedId(3);
    addAndMakeVisible(&wintypeCombo);

    wintypeAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "wintype", wintypeCombo));

    delayLabel.setText("Delay per bin", NotificationType::dontSendNotification);
    delayLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&delayLabel);

    delayMultiSlider.setup(multiSliderNumberOfPoints);
    delayMultiSlider.addListener(this);
    delayMultiSlider.setLookAndFeel(&plugexLookAndFeel);
    delayMultiSlider.setPoints(processor.delayPoints);
    addAndMakeVisible(&delayMultiSlider);

    feedbackLabel.setText("Feedback per bin", NotificationType::dontSendNotification);
    feedbackLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&feedbackLabel);

    feedbackMultiSlider.setup(multiSliderNumberOfPoints);
    feedbackMultiSlider.addListener(this);
    feedbackMultiSlider.setLookAndFeel(&plugexLookAndFeel);
    feedbackMultiSlider.setPoints(processor.feedbackPoints);
    addAndMakeVisible(&feedbackMultiSlider);

    startTimer(0.05);
}

Plugex_32_spectralDelayAudioProcessorEditor::~Plugex_32_spectralDelayAudioProcessorEditor()
{
    orderCombo.setLookAndFeel(nullptr);
    overlapsCombo.setLookAndFeel(nullptr);
    wintypeCombo.setLookAndFeel(nullptr);
    delayMultiSlider.setLookAndFeel(nullptr);
    feedbackMultiSlider.setLookAndFeel(nullptr);
}

void Plugex_32_spectralDelayAudioProcessorEditor::timerCallback() {
    if (processor.delayPointsChanged) {
        delayMultiSlider.setPoints(processor.delayPoints);
        processor.delayPointsChanged = false;
    }
    if (processor.feedbackPointsChanged) {
        feedbackMultiSlider.setPoints(processor.feedbackPoints);
        processor.feedbackPointsChanged = false;
    }
}

void Plugex_32_spectralDelayAudioProcessorEditor::multiSliderChanged(MultiSlider *multiSlider, const Array<float> &value) {
    if (multiSlider == &delayMultiSlider) {
        processor.setDelayPoints(value);
    } else if (multiSlider == &feedbackMultiSlider) {
        processor.setFeedbackPoints(value);
    }
}

//==============================================================================
void Plugex_32_spectralDelayAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_32_spectralDelayAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto area2 = area.removeFromTop(60);
    auto orderArea = area2.removeFromLeft(width / 3.0f);
    orderLabel.setBounds(orderArea.removeFromTop(20));
    orderCombo.setBounds(orderArea.removeFromTop(60).withSizeKeepingCentre(120, 24));

    auto overlapsArea = area2.removeFromLeft(width / 3.0f);
    overlapsLabel.setBounds(overlapsArea.removeFromTop(20));
    overlapsCombo.setBounds(overlapsArea.removeFromTop(60).withSizeKeepingCentre(120, 24));

    wintypeLabel.setBounds(area2.removeFromTop(20));
    wintypeCombo.setBounds(area2.removeFromTop(60).withSizeKeepingCentre(120, 24));

    area.removeFromTop(12);
    delayLabel.setBounds(area.removeFromTop(20).withSizeKeepingCentre(getWidth() - 20, 20));
    delayMultiSlider.setBounds(area.removeFromTop(120).withSizeKeepingCentre(multiSliderNumberOfPoints, 100));    

    area.removeFromTop(12);
    feedbackLabel.setBounds(area.removeFromTop(20).withSizeKeepingCentre(getWidth() - 20, 20));
    feedbackMultiSlider.setBounds(area.removeFromTop(120).withSizeKeepingCentre(multiSliderNumberOfPoints, 100));    
}
