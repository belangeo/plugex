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
Plugex_00_visualDesignAudioProcessorEditor::Plugex_00_visualDesignAudioProcessorEditor (Plugex_00_visualDesignAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (400, 600);

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("pink");

    title.setText("Plugex - common LookAndFeel !", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

    label.setText("This is a Label widget !", NotificationType::dontSendNotification);
    addAndMakeVisible(&label);

    knob.setLookAndFeel(&plugexLookAndFeel);
    knob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    knob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    knob.setNumDecimalPlacesToDisplay(3);
    addAndMakeVisible(&knob);

    hbar.setLookAndFeel(&plugexLookAndFeel);
    hbar.setSliderStyle(Slider::LinearBar);
    hbar.setNumDecimalPlacesToDisplay(3);
    addAndMakeVisible(&hbar);

    hslider.setLookAndFeel(&plugexLookAndFeel);
    hslider.setSliderStyle(Slider::LinearHorizontal);
    hslider.setTextBoxStyle(Slider::TextBoxRight, false, 80, 20);
    hslider.setNumDecimalPlacesToDisplay(3);
    addAndMakeVisible(&hslider);

    vbar.setLookAndFeel(&plugexLookAndFeel);
    vbar.setSliderStyle(Slider::LinearBarVertical);
    vbar.setNumDecimalPlacesToDisplay(3);
    addAndMakeVisible(&vbar);

    vslider.setLookAndFeel(&plugexLookAndFeel);
    vslider.setSliderStyle(Slider::LinearVertical);
    vslider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    vslider.setNumDecimalPlacesToDisplay(3);
    addAndMakeVisible(&vslider);

    combo.setLookAndFeel(&plugexLookAndFeel);
    combo.addItemList({"Frites", "Sauce", "Fromage"}, 1);
    combo.setSelectedId(1);
    addAndMakeVisible(&combo);

    button1.setLookAndFeel(&plugexLookAndFeel);
    button1.setButtonText("Simple button");
    addAndMakeVisible(&button1);

    button2.setLookAndFeel(&plugexLookAndFeel);
    button2.setButtonText("Two-state button");
    button2.setClickingTogglesState(true);
    addAndMakeVisible(&button2);

    toggle.setLookAndFeel(&plugexLookAndFeel);
    toggle.setButtonText("Toggle button");
    addAndMakeVisible(&toggle);

    multiSliderLabel.setText("This is a MultiSlider widget !", NotificationType::dontSendNotification);
    addAndMakeVisible(&multiSliderLabel);
    multiSlider.addListener(this);
    multiSlider.setLookAndFeel(&plugexLookAndFeel);
    addAndMakeVisible(&multiSlider);
}

Plugex_00_visualDesignAudioProcessorEditor::~Plugex_00_visualDesignAudioProcessorEditor()
{
}

void Plugex_00_visualDesignAudioProcessorEditor::multiSliderChanged(MultiSlider *multiSlider, const Array<float> &value) {
    std::cout << "Vector size = " << value.size() << std::endl;
}

//==============================================================================
void Plugex_00_visualDesignAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Plugex_00_visualDesignAudioProcessorEditor::resized()
{
    int width = getWidth();

    auto area = getLocalBounds().reduced(12, 12);

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

    label.setBounds(area.removeFromTop(24));
    area.removeFromTop(12);

    auto area2 = area.removeFromTop(128);

    vbar.setBounds(area2.removeFromLeft(48));
    area2.removeFromLeft(12);
    vslider.setBounds(area2.removeFromLeft(84));
    area2.removeFromLeft(12);
    knob.setBounds(area2.removeFromTop(84).removeFromLeft(84));

    area.removeFromTop(12);
    hbar.setBounds(area.removeFromTop(24));
    area.removeFromTop(12);
    hslider.setBounds(area.removeFromTop(24));
    area.removeFromTop(12);
    combo.setBounds(area.removeFromTop(24));
    area.removeFromTop(12);
    button1.setBounds(area.removeFromTop(24));
    area.removeFromTop(12);
    button2.setBounds(area.removeFromTop(24));
    area.removeFromTop(12);
    toggle.setBounds(area.removeFromTop(24));
    area.removeFromTop(12);
    multiSliderLabel.setBounds(area.removeFromTop(20));
    multiSlider.setBounds(area.withSizeKeepingCentre(width-20, 100));
}
