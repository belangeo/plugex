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

#include "FFTEngine.h"

FFTEngine::FFTEngine() {}

FFTEngine::~FFTEngine() {}

void FFTEngine::reset() {
    fifoIndex = 0;
    for (int overlap = 0; overlap < fftMaxOverlaps; overlap++) {
        zeromem(fftData[overlap], sizeof(fftData[overlap]));
        zeromem(fifo[overlap], sizeof(fifo[overlap]));
    }
    forwardFFT.reset( new dsp::FFT(fftOrder));
    window.setup(fftSize, (Windowing::WindowType)fftWintype);
}

void FFTEngine::setOrder(int order) {
    if (order == fftOrder)
        return;

    fftOrder = order <= fftMaxOrder ? order : fftMaxOrder;
    fftSize = 1 << fftOrder;
    fftHopSize = fftSize / fftOverlaps;
    reset();
}

void FFTEngine::setOverlaps(int overlaps) {
    if (overlaps == fftOverlaps)
        return;

    fftOverlaps = overlaps <= fftMaxOverlaps ? overlaps : fftMaxOverlaps;
    fftHopSize = fftSize / fftOverlaps; 
    reset();
}

void FFTEngine::setWintype(int type) {
    fftWintype = type;
    window.setup(fftSize, (Windowing::WindowType)fftWintype);
}

int FFTEngine::getSize() {
    return fftSize;
}

void FFTEngine::setup(int order, int overlaps, int wintype) {
    if (order == fftOrder && overlaps == fftOverlaps && wintype == fftWintype)
        return;

    fftOrder = order <= fftMaxOrder ? order : fftMaxOrder;
    fftSize = 1 << fftOrder;
    fftOverlaps = overlaps <= fftMaxOverlaps ? overlaps : fftMaxOverlaps;
    fftHopSize = fftSize / fftOverlaps; 
    fftWintype = wintype;
    reset();
}

void FFTEngine::computeFrame(int overlap) {
    zeromem (fftData[overlap], sizeof (fftData[overlap]));
    memcpy (fftData[overlap], fifo[overlap], sizeof (fifo[overlap]));
    window.multiplyWithWindowingTable (fftData[overlap], fftSize);
    forwardFFT->performRealOnlyForwardTransform (fftData[overlap], true);

    // Registered callback to process the FFT frame.
    listeners.call([&] (Listener& l) { l.fftEngineFrameReady(this, fftData[overlap], fftSize); });

    forwardFFT->performRealOnlyInverseTransform (fftData[overlap]);
    window.multiplyWithWindowingTable (fftData[overlap], fftSize);
}

float FFTEngine::process(float input) {
    if (fifoIndex % fftHopSize == 0) {
        computeFrame(fifoIndex / fftHopSize);
    }

    for (int overlap = 0; overlap < fftOverlaps; overlap++) {
        int fifoIndexOverlap = (fifoIndex - fftHopSize * overlap);
        fifoIndexOverlap = fifoIndexOverlap < 0 ? fifoIndexOverlap + fftSize : fifoIndexOverlap; 
        fifo[overlap][fifoIndexOverlap] = input;
    }

    float outputSignal = 0.0f;
    for (int overlap = 0; overlap < fftOverlaps; overlap++) {
        int fifoIndexOverlap = (fifoIndex - fftHopSize * overlap);
        fifoIndexOverlap = fifoIndexOverlap < 0 ? fifoIndexOverlap + fftSize : fifoIndexOverlap; 
        outputSignal += fftData[overlap][fifoIndexOverlap];
    }
    outputSignal /= fftOverlaps;

    fifoIndex++;
    if (fifoIndex == fftSize) {
        fifoIndex = 0;
    }

    return outputSignal;
}
