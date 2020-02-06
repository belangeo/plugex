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

#include <algorithm>
#include "DelayLine.h"

DelayLine::DelayLine() {}

DelayLine::~DelayLine() {}

void DelayLine::setup(float maxDelayTime, double sampleRate) {
    m_writePosition = 0;
    m_maxSize = static_cast<int> (maxDelayTime * sampleRate + 0.5);
    m_sampleRate = sampleRate;
    data.reset( new float[m_maxSize + 1] );
    std::fill(data.get(), data.get() + m_maxSize + 1, 0.f);
}

float DelayLine::read(float delayTime) {
    float delayTimeInSamples = delayTime * m_sampleRate;
    float readPosition = m_writePosition - delayTimeInSamples;
    if (readPosition < 0) {
        readPosition += m_maxSize;
    } else if (readPosition >= m_maxSize) {
        readPosition -= m_maxSize;
    }
    int readPosIntegerPart = static_cast<int>(readPosition);
    float readPosFloatPart = readPosition - readPosIntegerPart;
    float previous = data[readPosIntegerPart];
    float next = data[readPosIntegerPart + 1];
    return previous + (next - previous) * readPosFloatPart;
}

void DelayLine::write(float input) {
    data[m_writePosition] = input;
    if (m_writePosition == 0) {
        data[m_maxSize] = input;
    }
    m_writePosition++;
    if (m_writePosition == m_maxSize) {
        m_writePosition = 0;
    }
}
