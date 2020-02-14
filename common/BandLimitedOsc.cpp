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

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "BandLimitedOsc.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

BandLimitedOsc::BandLimitedOsc() {
    m_wavetype = 2;
    m_freq = 1.0;
    m_sharp = 0.0;
    m_pointer_pos = m_sah_pointer_pos = 0.0;
    srand((unsigned int)time(0));
    m_sah_last_value = 0.0;
    m_sah_current_value = (rand() / (float)RAND_MAX) * 2.0 - 1.0;
}

void BandLimitedOsc::setup(float sampleRate) {
    m_sampleRate = sampleRate;
    m_oneOverSr = 1.0 / m_sampleRate;
    m_twopi = 2 * M_PI;
    m_oneOverPiOverTwo = 1.0 / (M_PI / 2.0);
    m_srOverFour = m_sampleRate / 4.0;
    m_srOverEight = m_sampleRate / 8.0;
    m_pointer_pos = m_sah_pointer_pos = 0.0;
}

BandLimitedOsc::~BandLimitedOsc() {}

void BandLimitedOsc::reset() {
    m_pointer_pos = m_sah_pointer_pos = 0.0;
}

float BandLimitedOsc::_clip(float x) {
    if (x < 0.0) {
        x += 1.0;
    } else if (x >= 1.0) {
        x -= 1.0;
    }
    return x;
}

void BandLimitedOsc::setWavetype(int type) {
    if (type != m_wavetype) {
        type = type < 0 ? 0 : type > 5 ? 5 : type;
        m_wavetype = type;
    }
}

void BandLimitedOsc::setFreq(float freq) {
    m_freq = freq < 0.00001 ? 0.00001 : freq > m_srOverFour ? m_srOverFour : freq;
}

void BandLimitedOsc::setSharp(float sharp) {
    m_sharp = sharp < 0.0 ? 0.0 : sharp > 1.0 ? 1.0 : sharp;
}

float BandLimitedOsc::process() {
    float v1 = 0.0, v2 = 0.0, pointer = 0.0, numh = 0., maxHarms = 0., pos = 0.;
    float inc2 = 0.0, fade = 0.0, value = 0.;

    switch (m_wavetype) {
        // Sine
        case 0:
            numh = 0.0;
            pos = m_pointer_pos;
            if (pos >= 1.0)
                pos -= 1.0;
            value = sinf(m_twopi * pos);
            m_pointer_pos += m_freq * m_oneOverSr;
            m_pointer_pos = _clip(m_pointer_pos);
            break;
        // Triangle
        case 1:
            maxHarms = m_srOverFour / m_freq;
            if ((m_sharp * 36.0) > maxHarms)
                numh = maxHarms / 36.0;
            else
                numh = m_sharp;
            v1 = tanf(sinf(m_twopi * m_pointer_pos)) * m_oneOverPiOverTwo;
            pointer = m_pointer_pos + 0.25;
            if (pointer >= 1.0)
                pointer -= 1.0;
            v2 = 4.0 * (0.5 - fabsf(pointer - 0.5)) - 1.0;
            value = v1 + (v2 - v1) * numh;
            m_pointer_pos += m_freq * m_oneOverSr;
            m_pointer_pos = _clip(m_pointer_pos);
            break;
        // Square
        case 2:
            maxHarms = m_srOverEight / m_freq;
            numh = m_sharp * 46.0 + 4.0;
            if (numh > maxHarms)
                numh = maxHarms;
            value = atanf(numh * sinf(m_twopi * m_pointer_pos)) * m_oneOverPiOverTwo;
            m_pointer_pos += m_freq * m_oneOverSr;
            m_pointer_pos = _clip(m_pointer_pos);
            break;
        // Saw
        case 3:
            maxHarms = m_srOverFour / m_freq;
            numh = m_sharp * 46.0 + 4.0;
            if (numh > maxHarms)
                numh = maxHarms;
            pos = m_pointer_pos + 0.5;
            if (pos >= 1.0)
                pos -= 1.0;
            pos = pos * 2.0 - 1.0;
            value = -(pos - tanhf(numh * pos) / tanhf(numh));
            m_pointer_pos += m_freq * m_oneOverSr;
            m_pointer_pos = _clip(m_pointer_pos);
            break;
        // Ramp
        case 4:
            maxHarms = m_srOverFour / m_freq;
            numh = m_sharp * 46.0 + 4.0;
            if (numh > maxHarms)
                numh = maxHarms;
            pos = m_pointer_pos + 0.5;
            if (pos >= 1.0)
                pos -= 1.0;
            pos = pos * 2.0 - 1.0;
            value = pos - tanhf(numh * pos) / tanhf(numh);
            m_pointer_pos += m_freq * m_oneOverSr;
            m_pointer_pos = _clip(m_pointer_pos);
            break;
        // SAH
        case 5:
            numh = 1.0 - m_sharp;
            inc2 = 1.0 / (1.0 / (m_freq * m_oneOverSr) * numh);
            if (m_pointer_pos >= 1.0) {
                m_pointer_pos -= 1.0;
                m_sah_pointer_pos = 0.0;
                m_sah_last_value = m_sah_current_value;
                m_sah_current_value = (rand() / (float)RAND_MAX) * 2.0 - 1.0;
            }
            if (m_sah_pointer_pos < 1.0) {
                fade = 0.5 * sinf(M_PI * (m_sah_pointer_pos + 0.5)) + 0.5;
                value = m_sah_current_value + (m_sah_last_value - m_sah_current_value) * fade;
                m_sah_pointer_pos += inc2;
            }
            else {
                value = m_sah_current_value;
            }
            m_pointer_pos += m_freq * m_oneOverSr;
            break;
        default:
            value = 0.0;
            break;
    }
    return value;
}
