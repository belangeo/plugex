/*******************************************************************************
*
* Plugex - PLUGin EXamples
* Author: Olivier Bélanger
*
* Ce fichier d'en-tête est destiné à être utilisé par tous les plugiciels de
* l'ensemble Plugex. Il définit le style visuel commun à tous les plugiciels.
*
* La méthode setTheme(String newTheme) peut être appelée pour définir la couleur
* dominante du thème. Les valeurs possibles sont: blue, grey, red, green, orange
* ou purple.
*
*******************************************************************************/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// Notre LookAndFeel hérite de la version 4 du LookAndFeel par défaut de Juce.
// Cette classe n'a donc besoin de définir que les méthodes nécessaires pour 
// modifier l'apparence des objets graphiques utilisés par l'ensemble de plugiciels.
class PlugexLookAndFeel : public LookAndFeel_V4
{
public:
    // Le thème par defaut, définit dans le constructeur est le thème bleu.
    PlugexLookAndFeel() {
        setTheme("blue");
    }

    // Méthode permettant de changer le thème visuel du plugiciel. Les différents thèmes
    // se différencient par leur couleur dominante.
    bool setTheme(String newTheme) {
        const StringArray themes = StringArray("blue", "green", "red", "orange", "purple", "grey", "steal",
                                               "lightblue", "lightgreen", "pink");
        if (! themes.contains(newTheme)) {
            return false;
        }

        //Colour darktheme, lighttheme;
        Colour light = Colour(212, 211, 224);

        if (newTheme == "blue") {
            darktheme = Colour(6, 28, 64);
            lighttheme = Colour(113, 137, 209);
        } else if (newTheme == "grey") {
            darktheme = Colour(87, 87, 87);
            lighttheme = Colour(209, 209, 209);
        } else if (newTheme == "red") {
            darktheme = Colour(61, 7, 7);
            lighttheme = Colour(232, 70, 70);
        } else if (newTheme == "green") {
            darktheme = Colour(6, 54, 20);
            lighttheme = Colour(24, 191, 26);
        } else if (newTheme == "orange") {
            darktheme = Colour(56, 37, 7);
            lighttheme = Colour(191, 127, 23);
        } else if (newTheme == "purple") {
            darktheme = Colour(48, 6, 54);
            lighttheme = Colour(190, 25, 212);
        } else if (newTheme == "steal") {
            darktheme = Colour(35, 46, 54);
            lighttheme = Colour(118, 154, 181);
        } else if (newTheme == "lightblue") {
            darktheme = Colour(13, 69, 65);
            lighttheme = Colour(33, 184, 174);
        } else if (newTheme == "lightgreen") {
            darktheme = Colour(39, 56, 5);
            lighttheme = Colour(120, 171, 17);
        } else if (newTheme == "pink") {
            darktheme = Colour(56, 6, 40);
            lighttheme = Colour(184, 18, 131);
        }

        // Fenêtre principale.
        setColour(ResizableWindow::backgroundColourId, darktheme);

        // Étiquette.
        setColour(Label::textColourId, light);
        setColour(Label::textWhenEditingColourId, light);
        setColour(Label::outlineWhenEditingColourId, Colours::transparentBlack);
        setColour(Label::backgroundWhenEditingColourId, Colours::transparentBlack);

        // Potentiomètre.
        setColour(Slider::textBoxTextColourId, light);
        setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
        setColour(Slider::textBoxHighlightColourId, lighttheme.withAlpha(0.5f));
        setColour(Slider::backgroundColourId, Colours::transparentBlack);
        setColour(Slider::rotarySliderOutlineColourId, lighttheme);
        setColour(Slider::rotarySliderFillColourId, lighttheme.withAlpha(0.25f));
        setColour(Slider::trackColourId, lighttheme.withAlpha(0.25f));
        setColour(Slider::thumbColourId, lighttheme);

        // Menu déroulant.
        setColour(ComboBox::backgroundColourId, Colours::transparentBlack);
        setColour(ComboBox::textColourId, light);
        setColour(ComboBox::outlineColourId, lighttheme);
        setColour(ComboBox::buttonColourId, lighttheme);
        setColour(ComboBox::arrowColourId, lighttheme);
        setColour(ComboBox::focusedOutlineColourId, lighttheme);
        setColour(PopupMenu::backgroundColourId, darktheme);
        setColour(PopupMenu::textColourId, light);
        setColour(PopupMenu::highlightedBackgroundColourId, lighttheme.withAlpha(0.25f));
        setColour(PopupMenu::highlightedTextColourId, light);

        // Boutons.
        setColour(TextButton::buttonColourId, darktheme);
        setColour(TextButton::buttonOnColourId, lighttheme.withAlpha(0.25f));
        setColour(TextButton::textColourOnId, light);
        setColour(TextButton::textColourOffId, light);
        setColour(ToggleButton::textColourId, light);
        setColour(ToggleButton::tickColourId, lighttheme);
        setColour(ToggleButton::tickDisabledColourId, lighttheme.withAlpha(0.25f));

        return true;
    }

    Colour getDarkTheme() {
        return darktheme;
    }

    Colour getLightTheme() {
        return lighttheme;
    }

    Colour getLightFadeTheme() {
        return lighttheme.withAlpha(0.25f);
    }

    // Redéfinition de la méthode pour dessiner un potentiomètre en forme de bouton rond (knob).
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) {
        auto bounds = Rectangle<int> ((width - height) / 2.0f, y, height, height).toFloat().reduced (10);

        auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = jmin (1.5f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;

        Path background;
        background.addEllipse (bounds);
        g.setColour (slider.findColour (Slider::rotarySliderOutlineColourId));
        g.strokePath (background, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::rounded));

        if (slider.isEnabled()) {
            Path value;
            value.addPieSegment (bounds, rotaryStartAngle, toAngle, 0.0f);
            g.setColour (slider.findColour (Slider::rotarySliderFillColourId));
            g.fillPath (value);
        }

        Path thumb;
        Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - MathConstants<float>::halfPi),
                                 bounds.getCentreY() + arcRadius * std::sin (toAngle - MathConstants<float>::halfPi));

        g.setColour (slider.findColour (Slider::thumbColourId));
        thumb.addLineSegment (Line<float> (bounds.getCentre(), thumbPoint), 1.5f);
        g.strokePath(thumb, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::rounded));
    }
    
    // Redéfinition de la méthode pour dessiner un potentiomètre linéaire (traditionnel vertical ou horizontal et
    // rectangulaire plein avec la valeur écrite dans le potentiomètre, vertical ou horizontal).
    void drawLinearSlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                           float minSliderPos, float maxSliderPos, const Slider::SliderStyle style,
                           Slider& slider) {
        if (slider.isBar()) {
            g.setColour (slider.findColour (Slider::rotarySliderOutlineColourId));
            g.drawRect(Rectangle<float>(0.f, 0.f, width, height));
            if (slider.isHorizontal()) {
                float linePos = sliderPos < 0.0f ? 0.0f : sliderPos > width ? width : sliderPos;
                g.drawLine(Line<float>(linePos, y + 1.0f, linePos, height - 2.0f));
                g.setColour (slider.findColour (Slider::trackColourId));
                g.fillRect(Rectangle<float> (static_cast<float> (x), y + 1.0f, sliderPos - x - 1.0f, height - 2.0f));
            } else {
                float linePos = sliderPos < 0.0f ? 0.0f : sliderPos > height ? height : sliderPos;
                g.drawLine(Line<float>(x = 1.0f, linePos, width - 2.0f, linePos));
                g.setColour (slider.findColour (Slider::trackColourId));
                g.fillRect(Rectangle<float>(x + 1.0f, sliderPos, width - 2.0f, y + (height - sliderPos - 2.0f)));
            }
        }
        else {
            auto isTwoVal   = (style == Slider::SliderStyle::TwoValueVertical   || style == Slider::SliderStyle::TwoValueHorizontal);
            auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);

            auto trackWidth = jmin (6.0f, slider.isHorizontal() ? height * 0.25f : width * 0.25f);

            Point<float> startPoint (slider.isHorizontal() ? x : x + width * 0.5f,
                                     slider.isHorizontal() ? y + height * 0.5f : height + y);

            Point<float> endPoint (slider.isHorizontal() ? width + x : startPoint.x,
                                   slider.isHorizontal() ? startPoint.y : y);

            Rectangle<float> track = slider.isHorizontal() ? Rectangle<float>(x, y, width, height).withSizeKeepingCentre(width, height * 0.25f) :
                                                             Rectangle<float>(x, y, width, height).withSizeKeepingCentre(jmin(6.0f, width * 0.1f), height);
            g.setColour (slider.findColour (Slider::rotarySliderOutlineColourId));
            g.drawRect(track);

            Point<float> minPoint, maxPoint, thumbPoint;

            if (isTwoVal || isThreeVal)
            {
                minPoint = { slider.isHorizontal() ? minSliderPos : width * 0.5f,
                             slider.isHorizontal() ? height * 0.5f : minSliderPos };

                if (isThreeVal)
                    thumbPoint = { slider.isHorizontal() ? sliderPos : width * 0.5f,
                                   slider.isHorizontal() ? height * 0.5f : sliderPos };

                maxPoint = { slider.isHorizontal() ? maxSliderPos : width * 0.5f,
                             slider.isHorizontal() ? height * 0.5f : maxSliderPos };
            }
            else
            {
                auto kx = slider.isHorizontal() ? sliderPos : (x + width * 0.5f);
                auto ky = slider.isHorizontal() ? (y + height * 0.5f) : sliderPos;

                minPoint = startPoint;
                maxPoint = { kx, ky };
            }

            auto thumbWidth = slider.isHorizontal() ? 6.0f : width * 0.33f;
            auto thumbHeight = slider.isHorizontal() ? height * 0.8f : 6.0f;

            if (! isTwoVal)
            {
                g.setColour (slider.findColour (Slider::thumbColourId));
                g.fillRect (Rectangle<float> (thumbWidth, thumbHeight).withCentre (isThreeVal ? thumbPoint : maxPoint));
            }

            if (isTwoVal || isThreeVal)
            {
                auto sr = jmin (trackWidth, (slider.isHorizontal() ? height : width) * 0.4f);
                auto pointerColour = slider.findColour (Slider::thumbColourId);

                if (slider.isHorizontal())
                {
                    drawPointer (g, minSliderPos - sr,
                                 jmax (0.0f, y + height * 0.5f - trackWidth * 2.0f),
                                 trackWidth * 2.0f, pointerColour, 2);

                    drawPointer (g, maxSliderPos - trackWidth,
                                 jmin (y + height - trackWidth * 2.0f, y + height * 0.5f),
                                 trackWidth * 2.0f, pointerColour, 4);
                }
                else
                {
                    drawPointer (g, jmax (0.0f, x + width * 0.5f - trackWidth * 2.0f),
                                 minSliderPos - trackWidth,
                                 trackWidth * 2.0f, pointerColour, 1);

                    drawPointer (g, jmin (x + width - trackWidth * 2.0f, x + width * 0.5f), maxSliderPos - sr,
                                 trackWidth * 2.0f, pointerColour, 3);
                }
            }
        }
    }

    // Redéfinition de la méthode pour dessiner un menu déroulant.
    void drawComboBox (Graphics& g, int width, int height, bool, int, int, int, int, ComboBox& box) {
        Rectangle<int> boxBounds (0, 0, width, height);

        g.setColour (box.findColour (ComboBox::backgroundColourId));
        g.fillRect (boxBounds.toFloat());

        g.setColour (box.findColour (ComboBox::outlineColourId));
        g.drawRect (boxBounds.toFloat());

        Rectangle<int> arrowZone (width - 30, 0, 20, height);
        Path path;
        path.startNewSubPath (arrowZone.getX() + 3.0f, arrowZone.getCentreY() - 3.0f);
        path.lineTo (static_cast<float> (arrowZone.getCentreX()), arrowZone.getCentreY() + 3.0f);
        path.lineTo (arrowZone.getRight() - 3.0f, arrowZone.getCentreY() - 3.0f);

        g.setColour (box.findColour (ComboBox::arrowColourId).withAlpha ((box.isEnabled() ? 0.9f : 0.2f)));
        g.fillPath (path);
    }
    
    // Redéfinition de la méthode pour dessiner les boutons.
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
        auto bounds = button.getLocalBounds().toFloat();

        auto baseColour = backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
                                          .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f);

        if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
            baseColour = baseColour.contrasting (shouldDrawButtonAsDown ? 0.2f : 0.05f);

        g.setColour (baseColour);

        if (button.isConnectedOnLeft() || button.isConnectedOnRight())
        {
            Path path;
            path.addRectangle (bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());

            g.fillPath (path);

            g.setColour (button.findColour (ComboBox::outlineColourId));
            g.strokePath (path, PathStrokeType (1.0f));
        }
        else
        {
            g.fillRect (bounds);

            g.setColour (button.findColour (ComboBox::outlineColourId));
            g.drawRect (bounds, 1.0f);
        }
    }

    // Redéfinition de la méthode pour dessiner le bouton à deux états.
    void drawToggleButton (Graphics& g, ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
        auto fontSize = jmin (15.0f, button.getHeight() * 0.75f);
        auto tickWidth = fontSize * 1.1f;

        drawTickBox (g, button, 4.0f, (button.getHeight() - tickWidth) * 0.5f,
                     tickWidth, tickWidth,
                     button.getToggleState(),
                     button.isEnabled(),
                     shouldDrawButtonAsHighlighted,
                     shouldDrawButtonAsDown);

        g.setColour (button.findColour (ToggleButton::textColourId));
        g.setFont (fontSize);

        if (! button.isEnabled())
            g.setOpacity (0.5f);

        g.drawFittedText (button.getButtonText(),
                          button.getLocalBounds().withTrimmedLeft (roundToInt (tickWidth) + 10)
                                                 .withTrimmedRight (2),
                          Justification::centredLeft, 10);
    }

    // Redéfinition de la méthode pour dessiner la case à cocher du bouton à deux états.
    void drawTickBox (Graphics& g, Component& component, float x, float y, float w, float h,
                                      const bool ticked, const bool isEnabled,
                                      const bool shouldDrawButtonAsHighlighted,
                                      const bool shouldDrawButtonAsDown) {
        ignoreUnused (shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        Rectangle<float> tickBounds (x, y, w, h);

        g.setColour (component.findColour (isEnabled ? ToggleButton::tickColourId : ToggleButton::tickDisabledColourId));
        g.drawRect (tickBounds, 1.0f);

        if (ticked) {
            g.setColour (component.findColour (ToggleButton::tickColourId));
            g.fillRect(tickBounds.reduced(4.0f, 4.0f));
        }
    }

private:
    Colour darktheme;
    Colour lighttheme;
};