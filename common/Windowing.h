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

class Windowing 
{
public:

    enum WindowType
    {
        rectangular = 1,
        triangular,
        hanning,
        hamming,
        blackman,
        blackmanHarris4,
        blackmanHarris7,
        flatTop,
        halfSine,
        numWindowingMethods
    };

    Windowing();
    ~Windowing();

    void setup(int size, WindowType type);

    /** Multiplies the content of a buffer with the given window. */
    void multiplyWithWindowingTable (float *samples, int size) noexcept;

    /** Returns the name of a given windowing method. */
    static const char* getWindowingMethodName (WindowType type) noexcept;

private:
    Array<float> windowTable;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Windowing)
};