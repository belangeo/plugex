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

#include <cmath>
#include "Windowing.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

Windowing::Windowing() {}

Windowing::~Windowing() {}

void Windowing::setup(int size, WindowType type) {
    int i;
    float arg;

    windowTable.resize(size);
    float *window = windowTable.getRawDataPointer();

    switch (type) {
        case rectangular:
            for (i = 0; i < size; i++) {
                window[i] = 1.0f;
            }
            break;
        case triangular:
            arg = 2.0f / (size - 1);
            for (i = 0; i < (size - 1) / 2; i++) {
                window[i] = i * arg;
            }
            for ( ; i < size; i++) {
                window[i] = 2.0f - i * arg;
            }
            break;
        case hanning:
            arg = 2.0f * M_PI / (size - 1);
            for (i = 0; i < size; i++) {
                window[i] = 0.5 - 0.5 * cosf(arg * i);
            }
            break;
        case hamming:
            arg = 2.0f * M_PI / (size - 1);
            for (i = 0; i < size; i++) {
                window[i] = 0.54 - 0.46 * cosf(arg * i);
            }
            break;
        case blackman:
            arg = 2.0f * M_PI / (size - 1);
            for (i = 0; i < size; i++) {
                window[i] = 0.42323f - 0.49755f * cosf(arg * i) + 0.07922f * cosf(2.0f * arg * i);
            }
            break;
        case blackmanHarris4:
            arg = 2.0f * M_PI / (size - 1);
            for (i = 0; i < size; i++) {
                window[i] = 0.35875f - 0.48829f * cosf(arg * i) + 0.14128f * cosf(2.0f * arg * i) - \
                            0.01168f * cosf(3.0f * arg * i);
            }
            break;
        case blackmanHarris7:
            arg = 2.0f * M_PI / (size - 1);
            for (i = 0; i < size; i++) {
                window[i] = 0.2712203606f - 0.4334446123f * cosf(arg * i) + \
                            0.21800412f * cosf(2.0f * arg * i) - 0.0657853433f * cosf(3.0f * arg * i) + \
                            0.0107618673f * cosf(4.0f * arg * i) - 0.0007700127f * cosf(5.0f * arg * i) + \
                            0.00001368088f * cosf(6.0f * arg * i);
            }
            break;
        case flatTop:
            /* Tuckey (alpha = 0.66) */
            arg = 0.66f;
            for (i = 0; i < (int)(arg * size / 2.0f); i++) {
                window[i] = 0.5f * (1.0f + cosf(M_PI * (2.0f * i / (arg * size) - 1)));
            }
            for ( ; i < (int)(size * (1.0f - arg / 2)); i++) {
                window[i] = 1.0f;
            }
            for ( ; i < size; i++) {
                window[i] = 0.5f * (1.0f + cosf(M_PI * (2.0f * i / (arg * size) - 2.0f / arg + 1.0f)));
            }
            break;
        case halfSine:
            arg = M_PI / (size - 1);
            for (i = 0; i < size; i++) {
                window[i] = sinf(arg * i);
            }
            break;
        default:
            jassertfalse;
            break;
    }
}

void Windowing::multiplyWithWindowingTable (float *samples, int size) noexcept {
    FloatVectorOperations::multiply (samples, windowTable.getRawDataPointer(), jmin (size, windowTable.size()));
}

const char* Windowing::getWindowingMethodName (WindowType type) noexcept {
    switch (type) {
        case rectangular:       return "Rectangular";
        case triangular:        return "Triangular";
        case hanning:           return "Hanning";
        case hamming:           return "Hamming";
        case blackman:          return "Blackman";
        case blackmanHarris4:    return "Blackman-Harris 4 terms";
        case blackmanHarris7:    return "Blackman-Harris 7 terms";
        case flatTop:           return "Flat Top";
        case halfSine:          return "Half Sine";
        default: jassertfalse;  return "";
    }
}
