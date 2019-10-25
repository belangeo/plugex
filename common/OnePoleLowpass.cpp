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
#include "OnePoleLowpass.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

OnePoleLowpass::OnePoleLowpass() {
    m_sampleRate = 44100.0;
    m_freq = m_sampleRate / 2.0f;
    m_coeff = 0.0f;
}

OnePoleLowpass::~OnePoleLowpass() {}

void OnePoleLowpass::setup(double sampleRate) {
    m_sampleRate = sampleRate;
    m_y1 = 0.0f;
}

void OnePoleLowpass::setFreq(float freq) {
    if (freq != m_freq) {
        m_freq = freq;
        m_coeff = expf(-2.0f * M_PI * m_freq / m_sampleRate);
    }
}

float OnePoleLowpass::process(float input) {
    m_y1 = input + (m_y1 - input) * m_coeff;
    return m_y1;
}
