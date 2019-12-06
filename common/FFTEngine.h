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

#include "../JuceLibraryCode/JuceHeader.h"
#include "Windowing.h"

class FFTEngine 
{
public:
    FFTEngine();
    ~FFTEngine();
    void reset();
    void setup(int order, int overlaps, int wintype);
    void computeFrame(int overlap);
    float process(float input);

    void setOrder(int order);
    void setOverlaps(int overlaps);
    void setWintype(int type);

    int getSize();

    struct Listener
    {
        virtual ~Listener() {}
        virtual void fftEngineFrameReady(FFTEngine *engine, float *fftData, int fftSize) = 0;
    };

    void addListener(Listener* l) { listeners.add (l); }
    void removeListener(Listener* l) { listeners.remove (l); }

private:
    ListenerList<Listener> listeners;

    enum
    {
        fftMaxOrder = 14,
        fftMaxSize = 1 << fftMaxOrder,
        fftMaxOverlaps = 8
    };

    Windowing window;

    std::unique_ptr<dsp::FFT> forwardFFT;

    float fifo[fftMaxOverlaps][fftMaxSize];
    float fftData[fftMaxOverlaps][2 * fftMaxSize];
    int fifoIndex;
    int fftOrder;
    int fftSize;
    int fftOverlaps;
    int fftHopSize;
    int fftWintype;
};
