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

#include <memory>

class DelayLine {
    public:
        DelayLine();
        ~DelayLine();
        void setup(float maxDelayTime, double sampleRate);
        float read(float delayTime);
        void write(float input);

    private:
        double m_sampleRate;
        long m_maxSize;
        float m_writePosition;

        std::unique_ptr<float[]> data;
};
