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

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "PlugexLookAndFeel.h"

class Plugex_00_visualDesignAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Plugex_00_visualDesignAudioProcessorEditor (Plugex_00_visualDesignAudioProcessor&);
    ~Plugex_00_visualDesignAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    Plugex_00_visualDesignAudioProcessor& processor;

    PlugexLookAndFeel plugexLookAndFeel;

    Label title;
    Label label;
    Slider knob;
    Slider hbar;
    Slider hslider;
    Slider vbar;
    Slider vslider;
    ComboBox combo;
    TextButton button1;
    TextButton button2;
    ToggleButton toggle;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugex_00_visualDesignAudioProcessorEditor)
};
