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

class Biquad {
    public:
        Biquad();
        ~Biquad();
        void setup(double sampleRate);
        void setFreq(float freq);
        void setQ(float q);
        void setType(int type);
        void setParameters(float freq, float q, int type);
        void computeVariables();
        void computeCoefficients();
        float process(float input);

    private:
        double m_sampleRate;
        // Parameters
        float m_freq;
        float m_q;
        int m_type;
        // Variables
        float w0;
        float c;
        float alpha;
        // Coefficients
        float a0;
        float a1;
        float a2;
        float b0;
        float b1;
        float b2;
        // Last samples
        float x1;
        float x2;
        float y1;
        float y2;
};
