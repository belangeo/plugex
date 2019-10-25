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
#include "Biquad.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

Biquad::Biquad() {
    m_sampleRate = 44100.0;
    a0 = a1 = a2 = b0 = b1 = b2 = 0.0f;
}

Biquad::~Biquad() {}

void Biquad::setup(double sampleRate) {
    m_sampleRate = sampleRate;
    x1 = x2 = y1 = y2 = 0.0f;
    setParameters(1000.0f, 1.0f, 0);
}

void Biquad::setFreq(float freq) {
    if (freq != m_freq) {
        m_freq = freq < 0.1f ? 0.1 : freq > m_sampleRate / 2.0f ? m_sampleRate / 2.0f : freq;
        computeVariables();
    }
}

void Biquad::setQ(float q) {
    if (q != m_q) {
        m_q = q < 0.5f ? 0.5f : q > 100.0f ? 100.0f : q;
        computeVariables();
    }
}

void Biquad::setType(int type) {
    if (type != m_type) {
        m_type = type < 0 ? 0 : type > 4 ? 4 : type;
        computeCoefficients();
    }
}

void Biquad::setParameters(float freq, float q, int type) {
    if (freq != m_freq || q != m_q || type != m_type) {
        m_freq = freq < 0.1f ? 0.1 : freq > m_sampleRate / 2.0f ? m_sampleRate / 2.0f : freq;
        m_q = q < 0.5f ? 0.5f : q > 100.0f ? 100.0f : q;
        m_type = type < 0 ? 0 : type > 4 ? 4 : type;
        computeVariables();
    }
}

void Biquad::computeVariables() {
    w0 = m_freq * 2.0f * M_PI / m_sampleRate;
    c = cosf(w0);
    alpha = sinf(w0) / (2.0f * m_q);
    computeCoefficients();
}

void Biquad::computeCoefficients() {
    switch (m_type) {
        case 0: // lowpass
            b0 = b2 = (1.0f - c) / 2.0f;
            b1 = 1.0f - c;
            a0 = 1.0f / (1.0f + alpha);
            a1 = -2.0f * c;
            a2 = 1.0f - alpha;            
            break;
        case 1: // highpass
            b0 = (1.0f + c) / 2.0f;
            b1 = -(1.0f + c);
            b2 = b0;
            a0 = 1.0f / (1.0f + alpha);
            a1 = -2.0f * c;
            a2 = 1.0f - alpha;
            break;
        case 2: // bandpass
            b0 = alpha;
            b1 = 0.0f;
            b2 = -alpha;
            a0 = 1.0f / (1.0f + alpha);
            a1 = -2.0f * c;
            a2 = 1.0f - alpha;
            break;
        case 3: // bandstop
            b0 = 1.0f;
            b1 = a1 = -2.0f * c;
            b2 = 1.0f;
            a0 = 1.0f / (1.0f + alpha);
            a2 = 1.0f - alpha;
            break;
        case 4: // allpass
            b0 = a2 = 1.0f - alpha;
            b1 = a1 = -2.0f * c;
            b2 = 1.0f + alpha;
            a0 = 1.0f / (1.0f + alpha);
            break;
    }
}

float Biquad::process(float input) {
    float out = ( b0 * input + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2 ) * a0;
    x2 = x1; x1 = input; y2 = y1; y1 = out;
    return out;
}
