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
#include "SinOsc.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

SinOsc::SinOsc() {}

SinOsc::~SinOsc() {}

void SinOsc::setup(double sampleRate) {
    m_freq = m_phase = 0.0f;
    m_increment = m_runningPhase = 0.0f;
    m_sampleRate = sampleRate;
}

void SinOsc::setFreq(float freq) {
    if (freq != m_freq) {
        m_freq = freq;
        m_increment = m_freq / m_sampleRate;
    }
}

void SinOsc::setPhase(float phase) {
    m_phase = phase;
}

float SinOsc::process() {
    float value = sinf(m_runningPhase * M_PI * 2.0f + m_phase);
    m_runningPhase += m_increment;
    if (m_runningPhase >= 1.0f) {
        m_runningPhase -= 1.0f;
    }
    return value;
}
