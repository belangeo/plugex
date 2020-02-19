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
Plugex_39_waveModMidiSynthAudioProcessorEditor::Plugex_39_waveModMidiSynthAudioProcessorEditor (Plugex_39_waveModMidiSynthAudioProcessor& p,
                                                                                                  AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts),
      keyboardComponent (p.keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    setSize (700, 520);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("lightgreen");

    title.setText("Plugex - 39 - Wave Mod Midi Synth", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    attackLabel.setText("Attack", NotificationType::dontSendNotification);
    attackLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&attackLabel);

    attackKnob.setLookAndFeel(&plugexLookAndFeel);
    attackKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    attackKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&attackKnob);

    attackAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "attack", attackKnob));

    decayLabel.setText("Decay", NotificationType::dontSendNotification);
    decayLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&decayLabel);

    decayKnob.setLookAndFeel(&plugexLookAndFeel);
    decayKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    decayKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&decayKnob);

    decayAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "decay", decayKnob));

    sustainLabel.setText("Sustain", NotificationType::dontSendNotification);
    sustainLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&sustainLabel);

    sustainKnob.setLookAndFeel(&plugexLookAndFeel);
    sustainKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    sustainKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&sustainKnob);

    sustainAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "sustain", sustainKnob));

    releaseLabel.setText("Release", NotificationType::dontSendNotification);
    releaseLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&releaseLabel);

    releaseKnob.setLookAndFeel(&plugexLookAndFeel);
    releaseKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    releaseKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&releaseKnob);

    releaseAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "release", releaseKnob));

    //------------------------------------------------------------------------------------------------------------------
    lfotypeLabel.setText("LFO Wave Type", NotificationType::dontSendNotification);
    lfotypeLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&lfotypeLabel);

    lfotypeCombo.setLookAndFeel(&plugexLookAndFeel);
    lfotypeCombo.addItemList({"Sine", "Triangle", "Square", "Sawtooth", "Ramp", "Pulse", "Bipolar Pulse", "SampleAndHold"}, 1);
    lfotypeCombo.setSelectedId(1);
    addAndMakeVisible(&lfotypeCombo);

    lfotypeAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "lfotype", lfotypeCombo));

    lfofreqLabel.setText("LFO Freq", NotificationType::dontSendNotification);
    lfofreqLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&lfofreqLabel);

    lfofreqKnob.setLookAndFeel(&plugexLookAndFeel);
    lfofreqKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    lfofreqKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&lfofreqKnob);

    lfofreqAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "lfofreq", lfofreqKnob));

    lfodepthLabel.setText("LFO Depth", NotificationType::dontSendNotification);
    lfodepthLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&lfodepthLabel);

    lfodepthKnob.setLookAndFeel(&plugexLookAndFeel);
    lfodepthKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    lfodepthKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&lfodepthKnob);

    lfodepthAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "lfodepth", lfodepthKnob));

    lfoRouteFreq.setLookAndFeel(&plugexLookAndFeel);
    lfoRouteFreq.setButtonText("OSC Freq");
    addAndMakeVisible(&lfoRouteFreq);
    lfoRouteFreqAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfoRouteFreq", lfoRouteFreq));
    lfoRouteFreqInv.setLookAndFeel(&plugexLookAndFeel);
    lfoRouteFreqInv.setButtonText("inv");
    addAndMakeVisible(&lfoRouteFreqInv);
    lfoRouteFreqInvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfoRouteFreqInv", lfoRouteFreqInv));

    lfoRouteSharp.setLookAndFeel(&plugexLookAndFeel);
    lfoRouteSharp.setButtonText("OSC Bright");
    addAndMakeVisible(&lfoRouteSharp);
    lfoRouteSharpAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfoRouteSharp", lfoRouteSharp));
    lfoRouteSharpInv.setLookAndFeel(&plugexLookAndFeel);
    lfoRouteSharpInv.setButtonText("inv");
    addAndMakeVisible(&lfoRouteSharpInv);
    lfoRouteSharpInvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfoRouteSharpInv", lfoRouteSharpInv));

    lfoRouteGain.setLookAndFeel(&plugexLookAndFeel);
    lfoRouteGain.setButtonText("OSC Gain");
    addAndMakeVisible(&lfoRouteGain);
    lfoRouteGainAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfoRouteGain", lfoRouteGain));
    lfoRouteGainInv.setLookAndFeel(&plugexLookAndFeel);
    lfoRouteGainInv.setButtonText("inv");
    addAndMakeVisible(&lfoRouteGainInv);
    lfoRouteGainInvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfoRouteGainInv", lfoRouteGainInv));

    //------------------------------------------------------------------------------------------------------------------
    typeLabel.setText("Wave Type", NotificationType::dontSendNotification);
    typeLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&typeLabel);

    typeCombo.setLookAndFeel(&plugexLookAndFeel);
    typeCombo.addItemList({"Sine", "Triangle", "Square", "Sawtooth", "Ramp", "Pulse", "Bipolar Pulse", "SampleAndHold"}, 1);
    typeCombo.setSelectedId(1);
    addAndMakeVisible(&typeCombo);

    typeAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "type", typeCombo));

    sharpLabel.setText("Bright", NotificationType::dontSendNotification);
    sharpLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&sharpLabel);

    sharpKnob.setLookAndFeel(&plugexLookAndFeel);
    sharpKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    sharpKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&sharpKnob);

    sharpAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "sharp", sharpKnob));

    gainLabel.setText("Gain", NotificationType::dontSendNotification);
    gainLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&gainLabel);

    gainKnob.setLookAndFeel(&plugexLookAndFeel);
    gainKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    gainKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&gainKnob);

    gainAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "gain", gainKnob));

    stereoToggle.setLookAndFeel(&plugexLookAndFeel);
    stereoToggle.setButtonText("Stereo");
    addAndMakeVisible(&stereoToggle);
    stereoToggleAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "stereo", stereoToggle));

    //------------------------------------------------------------------------------------------------------------------
    addAndMakeVisible(keyboardComponent);
}

Plugex_39_waveModMidiSynthAudioProcessorEditor::~Plugex_39_waveModMidiSynthAudioProcessorEditor()
{
    attackKnob.setLookAndFeel(nullptr);
    decayKnob.setLookAndFeel(nullptr);
    sustainKnob.setLookAndFeel(nullptr);
    releaseKnob.setLookAndFeel(nullptr);
    lfotypeCombo.setLookAndFeel(nullptr);
    lfofreqKnob.setLookAndFeel(nullptr);
    lfodepthKnob.setLookAndFeel(nullptr);
    lfoRouteFreq.setLookAndFeel(nullptr);
    lfoRouteFreqInv.setLookAndFeel(nullptr);
    lfoRouteSharp.setLookAndFeel(nullptr);
    lfoRouteSharpInv.setLookAndFeel(nullptr);
    lfoRouteGain.setLookAndFeel(nullptr);
    lfoRouteGainInv.setLookAndFeel(nullptr);
    typeCombo.setLookAndFeel(nullptr);
    sharpKnob.setLookAndFeel(nullptr);
    gainKnob.setLookAndFeel(nullptr);
    stereoToggle.setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_39_waveModMidiSynthAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_39_waveModMidiSynthAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto area1 = area.removeFromTop(100);

    auto attackArea = area1.removeFromLeft(width/6.0f).withSizeKeepingCentre(80, 100);
    attackLabel.setBounds(attackArea.removeFromTop(20));
    attackKnob.setBounds(attackArea);

    auto decayArea = area1.removeFromLeft(width/6.0f).withSizeKeepingCentre(80, 100);
    decayLabel.setBounds(decayArea.removeFromTop(20));
    decayKnob.setBounds(decayArea);

    auto sustainArea = area1.removeFromLeft(width/6.0f).withSizeKeepingCentre(80, 100);
    sustainLabel.setBounds(sustainArea.removeFromTop(20));
    sustainKnob.setBounds(sustainArea);

    auto releaseArea = area1.removeFromLeft(width/6.0f).withSizeKeepingCentre(80, 100);
    releaseLabel.setBounds(releaseArea.removeFromTop(20));
    releaseKnob.setBounds(releaseArea);

    auto area2 = area.removeFromTop(140);

    auto lfotypeArea = area2.removeFromLeft(width/3.f).withSizeKeepingCentre(200, 100);
    lfotypeLabel.setBounds(lfotypeArea.removeFromTop(20));
    lfotypeCombo.setBounds(lfotypeArea.removeFromTop(60).withSizeKeepingCentre(200, 24));

    auto lfofreqArea = area2.removeFromLeft(width/6.0f).withSizeKeepingCentre(80, 100);
    lfofreqLabel.setBounds(lfofreqArea.removeFromTop(20));
    lfofreqKnob.setBounds(lfofreqArea);

    auto lfodepthArea = area2.removeFromLeft(width/6.0f).withSizeKeepingCentre(80, 100);
    lfodepthLabel.setBounds(lfodepthArea.removeFromTop(20));
    lfodepthKnob.setBounds(lfodepthArea);

    auto lfoRouteArea = area2.removeFromLeft(width/3.0f).withSizeKeepingCentre(200, 100);
    auto lfoRouteAreaItemHeight = lfoRouteArea.getHeight() / 3.f;

    auto lfoRouteFreqArea = lfoRouteArea.removeFromTop(lfoRouteAreaItemHeight);
    lfoRouteFreq.setBounds(lfoRouteFreqArea.removeFromLeft(130));
    lfoRouteFreqInv.setBounds(lfoRouteFreqArea);

    auto lfoRouteSharpArea = lfoRouteArea.removeFromTop(lfoRouteAreaItemHeight);
    lfoRouteSharp.setBounds(lfoRouteSharpArea.removeFromLeft(130));
    lfoRouteSharpInv.setBounds(lfoRouteSharpArea);

    auto lfoRouteGainArea = lfoRouteArea.removeFromTop(lfoRouteAreaItemHeight);
    lfoRouteGain.setBounds(lfoRouteGainArea.removeFromLeft(130));
    lfoRouteGainInv.setBounds(lfoRouteGainArea);

    auto area3 = area.removeFromTop(100);

    auto typeArea = area3.removeFromLeft(width/3.0f).withSizeKeepingCentre(80, 100);
    typeLabel.setBounds(typeArea.removeFromTop(20));
    typeCombo.setBounds(typeArea.removeFromTop(60).withSizeKeepingCentre(200, 24));

    auto sharpArea = area3.removeFromLeft(width/6.0f).withSizeKeepingCentre(80, 100);
    sharpLabel.setBounds(sharpArea.removeFromTop(20));
    sharpKnob.setBounds(sharpArea);

    auto gainArea = area3.removeFromLeft(width/6.0f).withSizeKeepingCentre(80, 100);
    gainLabel.setBounds(gainArea.removeFromTop(20));
    gainKnob.setBounds(gainArea);

    auto stereoToggleArea = area3.removeFromLeft(width/3.0f).withSizeKeepingCentre(200, 100);
    stereoToggle.setBounds(stereoToggleArea.withSizeKeepingCentre(stereoToggleArea.getWidth(), 20.f));

    area.removeFromTop(12);

    keyboardComponent.setBounds(area.removeFromBottom(80));
}
