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

#pragma once

class BandLimitedOsc {
    public:
        BandLimitedOsc();
        ~BandLimitedOsc();
        void setup(float sampleRate);
        void setWavetype(int type);
        void setFreq(float freq);
        void setSharp(float sharp);
        void setPhase(float phase);
        void reset();
        float process();
    private:
        // globals
        float m_sampleRate;
        float m_oneOverSr;
        float m_twopi;
        float m_oneOverPiOverTwo;
        float m_srOverFour;
        float m_srOverEight;

        // parameters
        // 0 = Sine, 1 = Triangle, 2 = Square, 3 = Saw,
        // 4 = Ramp, 5 = pulse, 6 = bi-pulse, 7 = SAH
        int m_wavetype;
        float m_freq;
        float m_sharp;
        float m_pointer_pos;
        float m_sah_pointer_pos;
        float m_sah_last_value;
        float m_sah_current_value;

        // private methods
        float _clip(float x);
};
