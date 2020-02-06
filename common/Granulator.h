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

#include <vector>
#include <memory>

#include <stdlib.h>
#include <time.h>

class Granulator {
    public:
        Granulator();
        ~Granulator();
        void setup(double sampleRate, double memorySize);
        float process(float input);

        void setRecordingSize(double newRecordingSize);
        void setRecording(bool shouldBeRecording);
        bool getIsRecording();

        void setDensity(float newDensity);
        void setPitch(float newPitch);
        void setPosition(float newPosition);
        void setDuration(float newDuration);
        void setDeviation(float newDeviation);

    private:
        double m_sampleRate;
        long maxSize;
        long recordingSize;
        long recordedSize;

        bool initialized;

        const int maxNumberOfGrains = 4096;

        bool isRecording;
        long recordingIndex;

        // Parameters
        float density;
        float pitch;
        float position;
        float duration;
        float deviation;

        float gainFactor;

        double timer;
        double deviationFactor;
        double oneOverSr;

        float numberOfActiveGrains;

        std::vector<float> gpos;
        std::vector<float> glen;
        std::vector<float> ginc;
        std::vector<float> gphs;
        std::vector<int> flags;

        std::unique_ptr<float[]> data;
};
