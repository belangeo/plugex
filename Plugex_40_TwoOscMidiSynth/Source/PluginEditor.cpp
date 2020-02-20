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
Plugex_40_twoOscMidiSynthAudioProcessorEditor::Plugex_40_twoOscMidiSynthAudioProcessorEditor (Plugex_40_twoOscMidiSynthAudioProcessor& p,
                                                                                                  AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts),
      keyboardComponent (p.keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    setSize (980, 750);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("lightgreen");

    title.setText("Plugex - 40 - Two Osc Midi Synth", NotificationType::dontSendNotification);
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
    lfo1typeLabel.setText("LFO 1 Wave Type", NotificationType::dontSendNotification);
    lfo1typeLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&lfo1typeLabel);

    lfo1typeCombo.setLookAndFeel(&plugexLookAndFeel);
    lfo1typeCombo.addItemList({"Sine", "Triangle", "Square", "Sawtooth", "Ramp", "Pulse", "Bipolar Pulse", "SampleAndHold"}, 1);
    lfo1typeCombo.setSelectedId(1);
    addAndMakeVisible(&lfo1typeCombo);

    lfo1typeAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "lfo1type", lfo1typeCombo));

    lfo1freqLabel.setText("LFO 1 Freq", NotificationType::dontSendNotification);
    lfo1freqLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&lfo1freqLabel);

    lfo1freqKnob.setLookAndFeel(&plugexLookAndFeel);
    lfo1freqKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    lfo1freqKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&lfo1freqKnob);

    lfo1freqAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "lfo1freq", lfo1freqKnob));

    lfo1depthLabel.setText("LFO 1 Depth", NotificationType::dontSendNotification);
    lfo1depthLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&lfo1depthLabel);

    lfo1depthKnob.setLookAndFeel(&plugexLookAndFeel);
    lfo1depthKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    lfo1depthKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&lfo1depthKnob);

    lfo1depthAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "lfo1depth", lfo1depthKnob));

    lfo1RouteLfo2Freq.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteLfo2Freq.setButtonText("LFO 2 Freq");
    addAndMakeVisible(&lfo1RouteLfo2Freq);
    lfo1RouteLfo2FreqAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteLfo2Freq", lfo1RouteLfo2Freq));
    lfo1RouteLfo2FreqInv.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteLfo2FreqInv.setButtonText("inv");
    addAndMakeVisible(&lfo1RouteLfo2FreqInv);
    lfo1RouteLfo2FreqInvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteLfo2FreqInv", lfo1RouteLfo2FreqInv));

    lfo1RouteLfo2Depth.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteLfo2Depth.setButtonText("LFO 2 Depth");
    addAndMakeVisible(&lfo1RouteLfo2Depth);
    lfo1RouteLfo2DepthAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteLfo2Depth", lfo1RouteLfo2Depth));
    lfo1RouteLfo2DepthInv.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteLfo2DepthInv.setButtonText("inv");
    addAndMakeVisible(&lfo1RouteLfo2DepthInv);
    lfo1RouteLfo2DepthInvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteLfo2DepthInv", lfo1RouteLfo2DepthInv));

    lfo1RouteFreq1.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteFreq1.setButtonText("OSC 1 Freq");
    addAndMakeVisible(&lfo1RouteFreq1);
    lfo1RouteFreq1Attachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteFreq1", lfo1RouteFreq1));
    lfo1RouteFreq1Inv.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteFreq1Inv.setButtonText("inv");
    addAndMakeVisible(&lfo1RouteFreq1Inv);
    lfo1RouteFreq1InvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteFreq1Inv", lfo1RouteFreq1Inv));

    lfo1RouteSharp1.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteSharp1.setButtonText("OSC 1 Bright");
    addAndMakeVisible(&lfo1RouteSharp1);
    lfo1RouteSharp1Attachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteSharp1", lfo1RouteSharp1));
    lfo1RouteSharp1Inv.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteSharp1Inv.setButtonText("inv");
    addAndMakeVisible(&lfo1RouteSharp1Inv);
    lfo1RouteSharp1InvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteSharp1Inv", lfo1RouteSharp1Inv));

    lfo1RouteGain1.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteGain1.setButtonText("OSC 1 Gain");
    addAndMakeVisible(&lfo1RouteGain1);
    lfo1RouteGain1Attachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteGain1", lfo1RouteGain1));
    lfo1RouteGain1Inv.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteGain1Inv.setButtonText("inv");
    addAndMakeVisible(&lfo1RouteGain1Inv);
    lfo1RouteGain1InvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteGain1Inv", lfo1RouteGain1Inv));

    lfo1RouteFreq2.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteFreq2.setButtonText("OSC 2 Freq");
    addAndMakeVisible(&lfo1RouteFreq2);
    lfo1RouteFreq2Attachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteFreq2", lfo1RouteFreq2));
    lfo1RouteFreq2Inv.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteFreq2Inv.setButtonText("inv");
    addAndMakeVisible(&lfo1RouteFreq2Inv);
    lfo1RouteFreq2InvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteFreq2Inv", lfo1RouteFreq2Inv));

    lfo1RouteSharp2.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteSharp2.setButtonText("OSC 2 Bright");
    addAndMakeVisible(&lfo1RouteSharp2);
    lfo1RouteSharp2Attachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteSharp2", lfo1RouteSharp2));
    lfo1RouteSharp2Inv.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteSharp2Inv.setButtonText("inv");
    addAndMakeVisible(&lfo1RouteSharp2Inv);
    lfo1RouteSharp2InvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteSharp2Inv", lfo1RouteSharp2Inv));

    lfo1RouteGain2.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteGain2.setButtonText("OSC 2 Gain");
    addAndMakeVisible(&lfo1RouteGain2);
    lfo1RouteGain2Attachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteGain2", lfo1RouteGain2));
    lfo1RouteGain2Inv.setLookAndFeel(&plugexLookAndFeel);
    lfo1RouteGain2Inv.setButtonText("inv");
    addAndMakeVisible(&lfo1RouteGain2Inv);
    lfo1RouteGain2InvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo1RouteGain2Inv", lfo1RouteGain2Inv));

    //------------------------------------------------------------------------------------------------------------------
    lfo2typeLabel.setText("LFO 2 Wave Type", NotificationType::dontSendNotification);
    lfo2typeLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&lfo2typeLabel);

    lfo2typeCombo.setLookAndFeel(&plugexLookAndFeel);
    lfo2typeCombo.addItemList({"Sine", "Triangle", "Square", "Sawtooth", "Ramp", "Pulse", "Bipolar Pulse", "SampleAndHold"}, 1);
    lfo2typeCombo.setSelectedId(1);
    addAndMakeVisible(&lfo2typeCombo);

    lfo2typeAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "lfo2type", lfo2typeCombo));

    lfo2freqLabel.setText("LFO 2 Freq", NotificationType::dontSendNotification);
    lfo2freqLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&lfo2freqLabel);

    lfo2freqKnob.setLookAndFeel(&plugexLookAndFeel);
    lfo2freqKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    lfo2freqKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&lfo2freqKnob);

    lfo2freqAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "lfo2freq", lfo2freqKnob));

    lfo2depthLabel.setText("LFO 2 Depth", NotificationType::dontSendNotification);
    lfo2depthLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&lfo2depthLabel);

    lfo2depthKnob.setLookAndFeel(&plugexLookAndFeel);
    lfo2depthKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    lfo2depthKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&lfo2depthKnob);

    lfo2depthAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "lfo2depth", lfo2depthKnob));

    lfo2RouteFreq1.setLookAndFeel(&plugexLookAndFeel);
    lfo2RouteFreq1.setButtonText("OSC 1 Freq");
    addAndMakeVisible(&lfo2RouteFreq1);
    lfo2RouteFreq1Attachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo2RouteFreq1", lfo2RouteFreq1));
    lfo2RouteFreq1Inv.setLookAndFeel(&plugexLookAndFeel);
    lfo2RouteFreq1Inv.setButtonText("inv");
    addAndMakeVisible(&lfo2RouteFreq1Inv);
    lfo2RouteFreq1InvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo2RouteFreq1Inv", lfo2RouteFreq1Inv));

    lfo2RouteSharp1.setLookAndFeel(&plugexLookAndFeel);
    lfo2RouteSharp1.setButtonText("OSC 1 Bright");
    addAndMakeVisible(&lfo2RouteSharp1);
    lfo2RouteSharp1Attachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo2RouteSharp1", lfo2RouteSharp1));
    lfo2RouteSharp1Inv.setLookAndFeel(&plugexLookAndFeel);
    lfo2RouteSharp1Inv.setButtonText("inv");
    addAndMakeVisible(&lfo2RouteSharp1Inv);
    lfo2RouteSharp1InvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo2RouteSharp1Inv", lfo2RouteSharp1Inv));

    lfo2RouteGain1.setLookAndFeel(&plugexLookAndFeel);
    lfo2RouteGain1.setButtonText("OSC 1 Gain");
    addAndMakeVisible(&lfo2RouteGain1);
    lfo2RouteGain1Attachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo2RouteGain1", lfo2RouteGain1));
    lfo2RouteGain1Inv.setLookAndFeel(&plugexLookAndFeel);
    lfo2RouteGain1Inv.setButtonText("inv");
    addAndMakeVisible(&lfo2RouteGain1Inv);
    lfo2RouteGain1InvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo2RouteGain1Inv", lfo2RouteGain1Inv));

    lfo2RouteFreq2.setLookAndFeel(&plugexLookAndFeel);
    lfo2RouteFreq2.setButtonText("OSC 2 Freq");
    addAndMakeVisible(&lfo2RouteFreq2);
    lfo2RouteFreq2Attachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo2RouteFreq2", lfo2RouteFreq2));
    lfo2RouteFreq2Inv.setLookAndFeel(&plugexLookAndFeel);
    lfo2RouteFreq2Inv.setButtonText("inv");
    addAndMakeVisible(&lfo2RouteFreq2Inv);
    lfo2RouteFreq2InvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo2RouteFreq2Inv", lfo2RouteFreq2Inv));

    lfo2RouteSharp2.setLookAndFeel(&plugexLookAndFeel);
    lfo2RouteSharp2.setButtonText("OSC 2 Bright");
    addAndMakeVisible(&lfo2RouteSharp2);
    lfo2RouteSharp2Attachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo2RouteSharp2", lfo2RouteSharp2));
    lfo2RouteSharp2Inv.setLookAndFeel(&plugexLookAndFeel);
    lfo2RouteSharp2Inv.setButtonText("inv");
    addAndMakeVisible(&lfo2RouteSharp2Inv);
    lfo2RouteSharp2InvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo2RouteSharp2Inv", lfo2RouteSharp2Inv));

    lfo2RouteGain2.setLookAndFeel(&plugexLookAndFeel);
    lfo2RouteGain2.setButtonText("OSC 2 Gain");
    addAndMakeVisible(&lfo2RouteGain2);
    lfo2RouteGain2Attachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo2RouteGain2", lfo2RouteGain2));
    lfo2RouteGain2Inv.setLookAndFeel(&plugexLookAndFeel);
    lfo2RouteGain2Inv.setButtonText("inv");
    addAndMakeVisible(&lfo2RouteGain2Inv);
    lfo2RouteGain2InvAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lfo2RouteGain2Inv", lfo2RouteGain2Inv));

    //------------------------------------------------------------------------------------------------------------------
    type1Label.setText("Osc 1 Wave Type", NotificationType::dontSendNotification);
    type1Label.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&type1Label);

    type1Combo.setLookAndFeel(&plugexLookAndFeel);
    type1Combo.addItemList({"Sine", "Triangle", "Square", "Sawtooth", "Ramp", "Pulse", "Bipolar Pulse", "SampleAndHold"}, 1);
    type1Combo.setSelectedId(1);
    addAndMakeVisible(&type1Combo);

    type1Attachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "type1", type1Combo));

    sharp1Label.setText("Bright", NotificationType::dontSendNotification);
    sharp1Label.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&sharp1Label);

    sharp1Knob.setLookAndFeel(&plugexLookAndFeel);
    sharp1Knob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    sharp1Knob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&sharp1Knob);

    sharp1Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "sharp1", sharp1Knob));

    gain1Label.setText("Gain", NotificationType::dontSendNotification);
    gain1Label.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&gain1Label);

    gain1Knob.setLookAndFeel(&plugexLookAndFeel);
    gain1Knob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    gain1Knob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&gain1Knob);

    gain1Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "gain1", gain1Knob));

    stereo1Toggle.setLookAndFeel(&plugexLookAndFeel);
    stereo1Toggle.setButtonText("Stereo");
    addAndMakeVisible(&stereo1Toggle);
    stereo1ToggleAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "stereo1", stereo1Toggle));

    //------------------------------------------------------------------------------------------------------------------
    type2Label.setText("Osc 2 Wave Type", NotificationType::dontSendNotification);
    type2Label.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&type2Label);

    type2Combo.setLookAndFeel(&plugexLookAndFeel);
    type2Combo.addItemList({"Sine", "Triangle", "Square", "Sawtooth", "Ramp", "Pulse", "Bipolar Pulse", "SampleAndHold"}, 1);
    type2Combo.setSelectedId(1);
    addAndMakeVisible(&type2Combo);

    type2Attachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "type2", type2Combo));

    sharp2Label.setText("Bright", NotificationType::dontSendNotification);
    sharp2Label.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&sharp2Label);

    sharp2Knob.setLookAndFeel(&plugexLookAndFeel);
    sharp2Knob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    sharp2Knob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&sharp2Knob);

    sharp2Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "sharp2", sharp2Knob));

    gain2Label.setText("Gain", NotificationType::dontSendNotification);
    gain2Label.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&gain2Label);

    gain2Knob.setLookAndFeel(&plugexLookAndFeel);
    gain2Knob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    gain2Knob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&gain2Knob);

    gain2Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "gain2", gain2Knob));

    stereo2Toggle.setLookAndFeel(&plugexLookAndFeel);
    stereo2Toggle.setButtonText("Stereo");
    addAndMakeVisible(&stereo2Toggle);
    stereo2ToggleAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "stereo2", stereo2Toggle));

    //------------------------------------------------------------------------------------------------------------------
    addAndMakeVisible(keyboardComponent);
}

Plugex_40_twoOscMidiSynthAudioProcessorEditor::~Plugex_40_twoOscMidiSynthAudioProcessorEditor()
{
    attackKnob.setLookAndFeel(nullptr);
    decayKnob.setLookAndFeel(nullptr);
    sustainKnob.setLookAndFeel(nullptr);
    releaseKnob.setLookAndFeel(nullptr);

    lfo1typeCombo.setLookAndFeel(nullptr);
    lfo1freqKnob.setLookAndFeel(nullptr);
    lfo1depthKnob.setLookAndFeel(nullptr);

    lfo1RouteLfo2Freq.setLookAndFeel(nullptr);
    lfo1RouteLfo2FreqInv.setLookAndFeel(nullptr);
    lfo1RouteLfo2Depth.setLookAndFeel(nullptr);
    lfo1RouteLfo2DepthInv.setLookAndFeel(nullptr);

    lfo1RouteFreq1.setLookAndFeel(nullptr);
    lfo1RouteFreq1Inv.setLookAndFeel(nullptr);
    lfo1RouteSharp1.setLookAndFeel(nullptr);
    lfo1RouteSharp1Inv.setLookAndFeel(nullptr);
    lfo1RouteGain1.setLookAndFeel(nullptr);
    lfo1RouteGain1Inv.setLookAndFeel(nullptr);
    lfo1RouteFreq2.setLookAndFeel(nullptr);
    lfo1RouteFreq2Inv.setLookAndFeel(nullptr);
    lfo1RouteSharp2.setLookAndFeel(nullptr);
    lfo1RouteSharp2Inv.setLookAndFeel(nullptr);
    lfo1RouteGain2.setLookAndFeel(nullptr);
    lfo1RouteGain2Inv.setLookAndFeel(nullptr);

    lfo2typeCombo.setLookAndFeel(nullptr);
    lfo2freqKnob.setLookAndFeel(nullptr);
    lfo2depthKnob.setLookAndFeel(nullptr);

    lfo2RouteFreq1.setLookAndFeel(nullptr);
    lfo2RouteFreq1Inv.setLookAndFeel(nullptr);
    lfo2RouteSharp1.setLookAndFeel(nullptr);
    lfo2RouteSharp1Inv.setLookAndFeel(nullptr);
    lfo2RouteGain1.setLookAndFeel(nullptr);
    lfo2RouteGain1Inv.setLookAndFeel(nullptr);
    lfo2RouteFreq2.setLookAndFeel(nullptr);
    lfo2RouteFreq2Inv.setLookAndFeel(nullptr);
    lfo2RouteSharp2.setLookAndFeel(nullptr);
    lfo2RouteSharp2Inv.setLookAndFeel(nullptr);
    lfo2RouteGain2.setLookAndFeel(nullptr);
    lfo2RouteGain2Inv.setLookAndFeel(nullptr);

    type1Combo.setLookAndFeel(nullptr);
    sharp1Knob.setLookAndFeel(nullptr);
    gain1Knob.setLookAndFeel(nullptr);
    stereo1Toggle.setLookAndFeel(nullptr);

    type2Combo.setLookAndFeel(nullptr);
    sharp2Knob.setLookAndFeel(nullptr);
    gain2Knob.setLookAndFeel(nullptr);
    stereo2Toggle.setLookAndFeel(nullptr);
}

//==============================================================================
void Plugex_40_twoOscMidiSynthAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_40_twoOscMidiSynthAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(12, 12);
    float width = area.getWidth();

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    auto area1 = area.removeFromTop(100);

    auto attackArea = area1.removeFromLeft(width/8.0f).withSizeKeepingCentre(80, 100);
    attackLabel.setBounds(attackArea.removeFromTop(20));
    attackKnob.setBounds(attackArea);

    auto decayArea = area1.removeFromLeft(width/8.0f).withSizeKeepingCentre(80, 100);
    decayLabel.setBounds(decayArea.removeFromTop(20));
    decayKnob.setBounds(decayArea);

    auto sustainArea = area1.removeFromLeft(width/8.0f).withSizeKeepingCentre(80, 100);
    sustainLabel.setBounds(sustainArea.removeFromTop(20));
    sustainKnob.setBounds(sustainArea);

    auto releaseArea = area1.removeFromLeft(width/8.0f).withSizeKeepingCentre(80, 100);
    releaseLabel.setBounds(releaseArea.removeFromTop(20));
    releaseKnob.setBounds(releaseArea);

    auto area2 = area.removeFromTop(140);

    auto lfo1typeArea = area2.removeFromLeft(width/4.f).withSizeKeepingCentre(200, 100);
    lfo1typeLabel.setBounds(lfo1typeArea.removeFromTop(20));
    lfo1typeCombo.setBounds(lfo1typeArea.removeFromTop(60).withSizeKeepingCentre(200, 24));

    auto lfo1freqArea = area2.removeFromLeft(width/8.0f).withSizeKeepingCentre(80, 100);
    lfo1freqLabel.setBounds(lfo1freqArea.removeFromTop(20));
    lfo1freqKnob.setBounds(lfo1freqArea);

    auto lfo1depthArea = area2.removeFromLeft(width/8.0f).withSizeKeepingCentre(80, 100);
    lfo1depthLabel.setBounds(lfo1depthArea.removeFromTop(20));
    lfo1depthKnob.setBounds(lfo1depthArea);

    auto lfo1RouteArea = area2.removeFromLeft(width/2.0f);
    auto lfo1RouteAreaItemHeight = lfo1RouteArea.getHeight() / 4.f;

    auto lfo1RouteAreaLeft = lfo1RouteArea.removeFromLeft(lfo1RouteArea.getWidth() / 2.f);

    auto lfo1RouteLfo2FreqArea = lfo1RouteAreaLeft.removeFromTop(lfo1RouteAreaItemHeight);
    lfo1RouteLfo2Freq.setBounds(lfo1RouteLfo2FreqArea.removeFromLeft(130));
    lfo1RouteLfo2FreqInv.setBounds(lfo1RouteLfo2FreqArea);

    auto lfo1RouteFreq1Area = lfo1RouteAreaLeft.removeFromTop(lfo1RouteAreaItemHeight);
    lfo1RouteFreq1.setBounds(lfo1RouteFreq1Area.removeFromLeft(130));
    lfo1RouteFreq1Inv.setBounds(lfo1RouteFreq1Area);

    auto lfo1RouteSharp1Area = lfo1RouteAreaLeft.removeFromTop(lfo1RouteAreaItemHeight);
    lfo1RouteSharp1.setBounds(lfo1RouteSharp1Area.removeFromLeft(130));
    lfo1RouteSharp1Inv.setBounds(lfo1RouteSharp1Area);

    auto lfo1RouteGain1Area = lfo1RouteAreaLeft.removeFromTop(lfo1RouteAreaItemHeight);
    lfo1RouteGain1.setBounds(lfo1RouteGain1Area.removeFromLeft(130));
    lfo1RouteGain1Inv.setBounds(lfo1RouteGain1Area);

    auto lfo1RouteAreaRight = lfo1RouteArea;

    auto lfo1RouteLfo2DepthArea = lfo1RouteAreaRight.removeFromTop(lfo1RouteAreaItemHeight);
    lfo1RouteLfo2Depth.setBounds(lfo1RouteLfo2DepthArea.removeFromLeft(130));
    lfo1RouteLfo2DepthInv.setBounds(lfo1RouteLfo2DepthArea);

    auto lfo1RouteFreq2Area = lfo1RouteAreaRight.removeFromTop(lfo1RouteAreaItemHeight);
    lfo1RouteFreq2.setBounds(lfo1RouteFreq2Area.removeFromLeft(130));
    lfo1RouteFreq2Inv.setBounds(lfo1RouteFreq2Area);

    auto lfo1RouteSharp2Area = lfo1RouteAreaRight.removeFromTop(lfo1RouteAreaItemHeight);
    lfo1RouteSharp2.setBounds(lfo1RouteSharp2Area.removeFromLeft(130));
    lfo1RouteSharp2Inv.setBounds(lfo1RouteSharp2Area);

    auto lfo1RouteGain2Area = lfo1RouteAreaRight.removeFromTop(lfo1RouteAreaItemHeight);
    lfo1RouteGain2.setBounds(lfo1RouteGain2Area.removeFromLeft(130));
    lfo1RouteGain2Inv.setBounds(lfo1RouteGain2Area);

    auto area3 = area.removeFromTop(140);

    auto lfo2typeArea = area3.removeFromLeft(width/4.f).withSizeKeepingCentre(200, 100);
    lfo2typeLabel.setBounds(lfo2typeArea.removeFromTop(20));
    lfo2typeCombo.setBounds(lfo2typeArea.removeFromTop(60).withSizeKeepingCentre(200, 24));

    auto lfo2freqArea = area3.removeFromLeft(width/8.0f).withSizeKeepingCentre(80, 100);
    lfo2freqLabel.setBounds(lfo2freqArea.removeFromTop(20));
    lfo2freqKnob.setBounds(lfo2freqArea);

    auto lfo2depthArea = area3.removeFromLeft(width/8.0f).withSizeKeepingCentre(80, 100);
    lfo2depthLabel.setBounds(lfo2depthArea.removeFromTop(20));
    lfo2depthKnob.setBounds(lfo2depthArea);

    auto lfo2RouteArea = area3.removeFromLeft(width/2.0f);
    auto lfo2RouteAreaItemHeight = lfo2RouteArea.getHeight() / 4.f;

    auto lfo2RouteAreaLeft = lfo2RouteArea.removeFromLeft(lfo2RouteArea.getWidth() / 2.f);

    lfo2RouteAreaLeft.removeFromTop(lfo2RouteAreaItemHeight);

    auto lfo2RouteFreq1Area = lfo2RouteAreaLeft.removeFromTop(lfo2RouteAreaItemHeight);
    lfo2RouteFreq1.setBounds(lfo2RouteFreq1Area.removeFromLeft(130));
    lfo2RouteFreq1Inv.setBounds(lfo2RouteFreq1Area);

    auto lfo2RouteSharp1Area = lfo2RouteAreaLeft.removeFromTop(lfo2RouteAreaItemHeight);
    lfo2RouteSharp1.setBounds(lfo2RouteSharp1Area.removeFromLeft(130));
    lfo2RouteSharp1Inv.setBounds(lfo2RouteSharp1Area);

    auto lfo2RouteGain1Area = lfo2RouteAreaLeft.removeFromTop(lfo2RouteAreaItemHeight);
    lfo2RouteGain1.setBounds(lfo2RouteGain1Area.removeFromLeft(130));
    lfo2RouteGain1Inv.setBounds(lfo2RouteGain1Area);

    auto lfo2RouteAreaRight = lfo2RouteArea;

    lfo2RouteAreaRight.removeFromTop(lfo2RouteAreaItemHeight);

    auto lfo2RouteFreq2Area = lfo2RouteAreaRight.removeFromTop(lfo2RouteAreaItemHeight);
    lfo2RouteFreq2.setBounds(lfo2RouteFreq2Area.removeFromLeft(130));
    lfo2RouteFreq2Inv.setBounds(lfo2RouteFreq2Area);

    auto lfo2RouteSharp2Area = lfo2RouteAreaRight.removeFromTop(lfo2RouteAreaItemHeight);
    lfo2RouteSharp2.setBounds(lfo2RouteSharp2Area.removeFromLeft(130));
    lfo2RouteSharp2Inv.setBounds(lfo2RouteSharp2Area);

    auto lfo2RouteGain2Area = lfo2RouteAreaRight.removeFromTop(lfo2RouteAreaItemHeight);
    lfo2RouteGain2.setBounds(lfo2RouteGain2Area.removeFromLeft(130));
    lfo2RouteGain2Inv.setBounds(lfo2RouteGain2Area);

    auto area4 = area.removeFromTop(100);

    auto type1Area = area4.removeFromLeft(width/4.0f).withSizeKeepingCentre(200, 100);
    type1Label.setBounds(type1Area.removeFromTop(20));
    type1Combo.setBounds(type1Area.removeFromTop(60).withSizeKeepingCentre(200, 24));

    auto sharp1Area = area4.removeFromLeft(width/8.0f).withSizeKeepingCentre(80, 100);
    sharp1Label.setBounds(sharp1Area.removeFromTop(20));
    sharp1Knob.setBounds(sharp1Area);

    auto gain1Area = area4.removeFromLeft(width/8.0f).withSizeKeepingCentre(80, 100);
    gain1Label.setBounds(gain1Area.removeFromTop(20));
    gain1Knob.setBounds(gain1Area);

    auto stereo1ToggleArea = area4.removeFromLeft(width/3.0f).withSizeKeepingCentre(200, 100);
    stereo1Toggle.setBounds(stereo1ToggleArea.withSizeKeepingCentre(stereo1ToggleArea.getWidth(), 20.f));

    auto area5 = area.removeFromTop(100);

    auto type2Area = area5.removeFromLeft(width/4.0f).withSizeKeepingCentre(200, 100);
    type2Label.setBounds(type2Area.removeFromTop(20));
    type2Combo.setBounds(type2Area.removeFromTop(60).withSizeKeepingCentre(200, 24));

    auto sharp2Area = area5.removeFromLeft(width/8.0f).withSizeKeepingCentre(80, 100);
    sharp2Label.setBounds(sharp2Area.removeFromTop(20));
    sharp2Knob.setBounds(sharp2Area);

    auto gain2Area = area5.removeFromLeft(width/8.0f).withSizeKeepingCentre(80, 100);
    gain2Label.setBounds(gain2Area.removeFromTop(20));
    gain2Knob.setBounds(gain2Area);

    auto stereo2ToggleArea = area5.removeFromLeft(width/3.0f).withSizeKeepingCentre(200, 100);
    stereo2Toggle.setBounds(stereo2ToggleArea.withSizeKeepingCentre(stereo2ToggleArea.getWidth(), 20.f));

    area.removeFromTop(12);

    keyboardComponent.setBounds(area.removeFromBottom(80));
}
