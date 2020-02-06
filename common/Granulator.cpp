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

#include <cmath>
#include "Granulator.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

Granulator::Granulator() {
    maxSize = recordingSize = 0;
    initialized = false;
}

Granulator::~Granulator() {}

void Granulator::setup(double sampleRate, double memorySize) {
    m_sampleRate = sampleRate;
    maxSize = recordingSize = static_cast<long> (m_sampleRate * memorySize);

    isRecording = false;
    recordingIndex = 0;

    oneOverSr = 1.0 / sampleRate;
    timer = 1.0;
    deviationFactor = 1.0;
    numberOfActiveGrains = 0;

    density = 50.f;
    pitch = 1.f;
    position = 0.f;
    duration = 0.1f;
    deviation = 0.f;

    gainFactor = sqrtf(sqrtf(density));

    data.reset( new float[maxSize + 1] );
    std::fill(data.get(), data.get() + maxSize + 1, 0.f);

    srand(time(NULL));
    gpos.resize(maxNumberOfGrains, 0.f);
    glen.resize(maxNumberOfGrains, 0.f);
    ginc.resize(maxNumberOfGrains, 0.f);
    gphs.resize(maxNumberOfGrains, 0.f);
    flags.resize(maxNumberOfGrains, 0);

    initialized = true;
}

void Granulator::setRecording(bool shouldBeRecording) {
    isRecording = shouldBeRecording;
    recordingIndex = 0;
    recordedSize = 0;
}

bool Granulator::getIsRecording() {
    return isRecording;
}

void Granulator::setDensity(float newDensity) {
    density = newDensity < 1.f ? 1.f : newDensity;
    gainFactor = sqrtf(sqrtf(density));
}

void Granulator::setPitch(float newPitch) {
    pitch = newPitch;
}

void Granulator::setPosition(float newPosition) {
    position = newPosition;
}

void Granulator::setDuration(float newDuration) {
    duration = newDuration;
}

void Granulator::setDeviation(float newDeviation) {
    deviation = newDeviation;
}

void Granulator::setRecordingSize(double newRecordingSize) {
    recordingSize = static_cast<long> (m_sampleRate * newRecordingSize);
    if (recordingSize > maxSize)
        recordingSize = maxSize;
}

float Granulator::process(float input) {
    if (! initialized) 
        return 0.f;

    if (isRecording && recordingIndex < recordingSize) {
        data[recordingIndex++] = input;
        if (recordingIndex == recordingSize) {
            isRecording = false;
        }
        recordedSize = recordingIndex - 1;
    }

    bool needNewGrain = false;
    double timeIncrement = density * oneOverSr * deviationFactor;
    timer += timeIncrement;
    if (timer >= 1.0) {
        timer -= 1.0;
        needNewGrain = true;
    }

    float out = 0.f;

    if (needNewGrain) {
        for (int j = 0; j < maxNumberOfGrains; j++) {
            if (flags[j] == 0) {
                flags[j] = 1;
                if (j >= numberOfActiveGrains)
                    numberOfActiveGrains = j + 1;
                gpos[j] = position * recordedSize;
                glen[j] = duration * m_sampleRate * pitch;
                if ((gpos[j] + glen[j]) >= recordedSize || (gpos[j] + glen[j]) < 0)
                    flags[j] = false;
                gphs[j] = 0.f;
                ginc[j] = 1.f / (duration * m_sampleRate);
                deviationFactor = (rand() / (float)RAND_MAX * 2.0 - 1.0) * deviation + 1.0;
                break;
            }
        }
    }

    for (int j = 0; j < numberOfActiveGrains; j++) {
        if (flags[j]) {
            float amp = cosf(M_PI * 2.f * gphs[j]) * -0.5f + 0.5f;
            float index = gphs[j] * glen[j] + gpos[j];
            int ipart = (int)index;
            float val = (data[ipart] + (data[ipart+1] - data[ipart]) * (index - ipart)) * amp;
            out += val;
            gphs[j] += ginc[j];
            if (gphs[j] >= 1.0)
                flags[j] = 0;
        }
    }

    return out / gainFactor;
}
