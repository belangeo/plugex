=============
Configuration
=============

- Ne pas oublier de cocher VST2.

- Ajouter le fichier common/PlugexLookAndFeel.h et le path "../../../common" dans Header Search Paths

- modules/juce_gui_basics, activer l'option JUCE_USE_XRENDER

Ne pas oublier "Company Name" dans les propriété du plugin, sinon il apparaît sous le nom "yourcompany".

=========
Copyright
=========

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

====================
À retirer...
====================
PluginEditor.cpp
----------------

paint() ->

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);

====================
Ajout du LookAndFeel
====================

PluginEditor.h
--------------

#include "PlugexLookAndFeel.h"

    PlugexLookAndFeel plugexLookAndFeel;

PluginEditor.cpp
----------------

    setLookAndFeel(&plugexLookAndFeel);
    plugexLookAndFeel.setTheme("blue");


========================
Ajout du titre du plugin
========================

PluginEditor.h
--------------

    Label title;

PluginEditor.cpp
----------------

    title.setText("Plugex - 01 - Amplitude Linear", NotificationType::dontSendNotification);
    title.setFont(title.getFont().withPointHeight(title.getFont().getHeightInPoints() + 4));
    title.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&title);

resized() ->

    auto area = getLocalBounds().reduced(12, 12);

    title.setBounds(area.removeFromTop(36));
    area.removeFromTop(12);

===================================
Ajout du gestionnaire de paramètres
===================================

PluginProcessor.h
-----------------

    AudioProcessorValueTreeState parameters;
    
    std::atomic<float> *gainParameter = nullptr;

PluginProcessor.cpp
-------------------

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("gain"), String("Gain"), String(),
                                                     NormalisableRange<float>(0.0f, 2.0f),
                                                     1.0f, nullptr, nullptr));

    return { parameters.begin(), parameters.end() };
}

    parameters (*this, nullptr, Identifier(JucePlugin_Name), createParameterLayout())

constructor ->

    gainParameter = parameters.getRawParameterValue("gain");

===========================
Traitement des échantillons
===========================

PluginProcessor.cpp (processBlock()
-----------------------------------

        for (int i = 0; i < buffer.getNumSamples(); i++) {
            channelData[i] *= *gainParameter;
        }


==============================
Ajout des contrôles graphiques
==============================

PluginEditor.h
--------------

Argument du constructeur ->

    , AudioProcessorValueTreeState& vts

private ->

    AudioProcessorValueTreeState& valueTreeState;

    Label  gainLabel;
    Slider gainKnob;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

PluginEditor.cpp
----------------

Argument du constructeur ->

    , AudioProcessorValueTreeState& vts

Initialisation du constructeur ->

    , valueTreeState (vts)

Constructeur ->

    gainLabel.setText("Gain", NotificationType::dontSendNotification);
    gainLabel.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(&gainLabel);

    gainKnob.setLookAndFeel(&plugexLookAndFeel);
    gainKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    gainKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(&gainKnob);

    gainAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "gain", gainKnob));

Destructeur ->

    gainKnob.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);

resized() ->

    auto gainArea = area.removeFromTop(100).withSizeKeepingCentre(80, 100);
    gainLabel.setBounds(gainArea.removeFromTop(20));
    gainKnob.setBounds(gainArea);
    area.removeFromTop(12);

PluginProcessor.cpp
-------------------

Argument à la création de l'éditeur dans la méthode createEditor() ->

    ... AudioProcessorEditor (*this, parameters);


===================================
Lissage des variations de paramètre
===================================

PluginProcessor.h
-----------------

private ->

    SmoothedValue<float> gainSmoothed;

PluginProcessor.cpp
-------------------

prepareToPlay() ->

    gainSmoothed.reset(sampleRate, samplesPerBlock/sampleRate);
    gainSmoothed.setCurrentAndTargetValue(*gainParameter);

processBlock() ->

    gainSmoothed.setTargetValue(*gainParameter);

            channelData[i] *= gainSmoothed.getNextValue();


=============================================
Affichage des valeurs des potentiomètres (dB)
=============================================

PluginProcessor.cpp
-------------------

static String gainSliderValueToText(float value) {
    float val = 20.0f * log10f(jmax(0.001f, value));
    return String(val, 2) + String(" dB");
}

static float gainSliderTextToValue(const String& text) {
    float val = jlimit(-60.0f, 18.0f, text.getFloatValue());
    return powf(10.0f, val * 0.05f);
}

    parameters.push_back(std::make_unique<Parameter>(String("gain"), String("Gain"), String(),
                                                     NormalisableRange<float>(0.001f, 7.94f, 0.001f, 0.3f),
                                                     1.0f, gainSliderValueToText, gainSliderTextToValue));



==========================================================
Remplacer une chaîne de caractères dans plusieurs fichiers
==========================================================

sed -i 's/original/new/g' *.*

 
