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

class OnePoleLowpass {
    public:
        OnePoleLowpass();
        ~OnePoleLowpass();
        void setup(double sampleRate);
        void setFreq(double freq);
        float process(float input);

    private:
        double m_sampleRate;
        float m_freq;
        float m_coeff;
        float m_y1;
};
