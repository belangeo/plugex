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
#include "PlugexLookAndFeel.h"

class MultiSlider : public Component
{
public:
    MultiSlider();
    ~MultiSlider();

    void setup(int numberOfPoints);

    void paint(Graphics&) override;
    void resized() override;

	void mouseDown (const MouseEvent &event);
 	void mouseDrag (const MouseEvent &event);

    void setPoints(const Array<float> &points);

    struct Listener
    {
        virtual ~Listener() {}
        virtual void multiSliderChanged(const Array<float> &value) = 0;
    };

    void addListener(Listener* l) { listeners.add (l); }
    void removeListener(Listener* l) { listeners.remove (l); }

private:
    ListenerList<Listener> listeners;

    Point<int> lastPosition;

    Array<float> bars;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiSlider)
};
