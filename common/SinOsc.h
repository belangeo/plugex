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

class SinOsc {
    public:
        SinOsc();
        ~SinOsc();
        void setup(double sampleRate);
        void setFreq(float freq);
        void setPhase(float phase);
        float process();

    private:
        double m_sampleRate;
        long m_freq;
        float m_phase;
        float m_increment;
        float m_runningPhase;
};
