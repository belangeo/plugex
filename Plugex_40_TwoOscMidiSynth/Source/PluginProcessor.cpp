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

#include "PluginProcessor.h"
#include "PluginEditor.h"

static const int numberOfVoices = 8;

//==============================================================================
MySynthesiserVoice::MySynthesiserVoice() {
    lfo1.setup(getSampleRate());
    lfo2.setup(getSampleRate());
    oscillator1Left.setup(getSampleRate());
    oscillator1Right.setup(getSampleRate());
    oscillator2Left.setup(getSampleRate());
    oscillator2Right.setup(getSampleRate());
    envelope.setSampleRate(getSampleRate());
    smoothedGain1.reset(256);
    smoothedGain2.reset(256);
}

bool MySynthesiserVoice::canPlaySound(SynthesiserSound *sound) {
    return dynamic_cast<MySynthesiserSound *> (sound) != nullptr;
}

void MySynthesiserVoice::startNote(int midiNoteNumber, float velocity,
                                   SynthesiserSound *, int /*currentPitchWheelPosition*/) {
    noteFreq = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    level = velocity * 0.15;
    envelope.noteOn();
    lfo1.reset();
    lfo2.reset();
    lfo1.setFreq(lfoFreq1);
    lfo2.setFreq(lfoFreq2);
    oscillator1Left.setFreq(noteFreq);
    oscillator1Left.setSharp(noteSharp1);
    oscillator1Right.setFreq(noteFreq * 1.003);
    oscillator1Right.setSharp(noteSharp1);
    oscillator2Left.setFreq(noteFreq);
    oscillator2Left.setSharp(noteSharp2);
    oscillator2Right.setFreq(noteFreq * 1.003);
    oscillator2Right.setSharp(noteSharp2);
}

void MySynthesiserVoice::stopNote(float /*velocity*/, bool allowTailOff) {
    envelope.noteOff();
}

void MySynthesiserVoice::renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) {
    while (--numSamples >= 0) {
        auto envAmp = envelope.getNextSample();

        auto lfoValue1 = lfo1.process() * lfoDepth1;

        if (lfo1lfo2freqSwith) {
            if (lfo1lfo2freqSwithInv) {
                lfo2.setFreq(lfoFreq2 * (1.f + -lfoValue1));
            } else {
                lfo2.setFreq(lfoFreq2 * (1.f + lfoValue1));
            }
        }

        float lfoValue2;
        if (lfo1lfo2depthSwith) {
            if (lfo1lfo2depthSwithInv) {
                lfoValue2 = lfo2.process() * lfoDepth2 * (1.f + -lfoValue1);
            } else {
                lfoValue2 = lfo2.process() * lfoDepth2 * (1.f + lfoValue1);
            }
        } else {
            lfoValue2 = lfo2.process() * lfoDepth2;
        }

        // Oscillator 1 ------------------------------------------------
        float osc1freq = noteFreq;
        if (lfo1freqSwitch1) {
            if (lfo1freqSwitch1Inv)
                osc1freq *= (1.f + -lfoValue1 * 0.25f);
            else
                osc1freq *= (1.f + lfoValue1 * 0.25f);
        }
        if (lfo2freqSwitch1) {
            if (lfo2freqSwitch1Inv)
                osc1freq *= (1.f + -lfoValue2 * 0.25f);
            else
                osc1freq *= (1.f + lfoValue2 * 0.25f);
        }
        oscillator1Left.setFreq(osc1freq);
        oscillator1Right.setFreq(osc1freq * 1.003);

        float osc1sharp = noteSharp1;
        if (lfo1sharpSwitch1) {
            if (lfo1sharpSwitch1Inv)
                osc1sharp *= (1.f + -lfoValue1);
            else
                osc1sharp *= (1.f + lfoValue1);
        }
        if (lfo2sharpSwitch1) {
            if (lfo2sharpSwitch1Inv)
                osc1sharp *= (1.f + -lfoValue2);
            else
                osc1sharp *= (1.f + lfoValue2);
        }
        oscillator1Left.setSharp(osc1sharp);
        oscillator1Right.setSharp(osc1sharp * 0.99);

        float currentSample = oscillator1Left.process() * level * envAmp;
        float currentSample2 = oscillator1Right.process() * level * envAmp;
        if (lfo1gainSwitch1) {
            if (lfo1gainSwitch1Inv) {
                currentSample *= (1.f + -lfoValue1);
                currentSample2 *= (1.f + -lfoValue1);
            } else {
                currentSample *= (1.f + lfoValue1);
                currentSample2 *= (1.f + lfoValue1);
            }
        }
        if (lfo2gainSwitch1) {
            if (lfo2gainSwitch1Inv) {
                currentSample *= (1.f + -lfoValue2);
                currentSample2 *= (1.f + -lfoValue2);
            } else {
                currentSample *= (1.f + lfoValue2);
                currentSample2 *= (1.f + lfoValue2);
            }
        }

        currentSample *= smoothedGain1.getNextValue();
        currentSample2 *= smoothedGain1.getNextValue();
        if (! stereoSwitch1)
            currentSample2 = currentSample;

        // Oscillator 2 ------------------------------------------------
        float osc2freq = noteFreq;
        if (lfo1freqSwitch2) {
            if (lfo1freqSwitch2Inv)
                osc2freq *= (1.f + -lfoValue1 * 0.25f);
            else
                osc2freq *= (1.f + lfoValue1 * 0.25f);
        }
        if (lfo2freqSwitch2) {
            if (lfo2freqSwitch2Inv)
                osc2freq *= (1.f + -lfoValue2 * 0.25f);
            else
                osc2freq *= (1.f + lfoValue2 * 0.25f);
        }
        oscillator2Left.setFreq(osc2freq);
        oscillator2Right.setFreq(osc2freq * 1.003);

        float osc2sharp = noteSharp2;
        if (lfo1sharpSwitch2) {
            if (lfo1sharpSwitch2Inv)
                osc2sharp *= (1.f + -lfoValue1);
            else
                osc2sharp *= (1.f + lfoValue1);
        }
        if (lfo2sharpSwitch2) {
            if (lfo2sharpSwitch2Inv)
                osc2sharp *= (1.f + -lfoValue2);
            else
                osc2sharp *= (1.f + lfoValue2);
        }
        oscillator2Left.setSharp(osc2sharp);
        oscillator2Right.setSharp(osc2sharp * 0.99);

        float osc2Sample = oscillator2Left.process() * level * envAmp;
        float osc2Sample2 = oscillator2Right.process() * level * envAmp;
        if (lfo1gainSwitch2) {
            if (lfo1gainSwitch2Inv) {
                osc2Sample *= (1.f + -lfoValue1);
                osc2Sample2 *= (1.f + -lfoValue1);
            } else {
                osc2Sample *= (1.f + lfoValue1);
                osc2Sample2 *= (1.f + lfoValue1);
            }
        }
        if (lfo2gainSwitch2) {
            if (lfo2gainSwitch2Inv) {
                osc2Sample *= (1.f + -lfoValue2);
                osc2Sample2 *= (1.f + -lfoValue2);
            } else {
                osc2Sample *= (1.f + lfoValue2);
                osc2Sample2 *= (1.f + lfoValue2);
            }
        }

        osc2Sample *= smoothedGain2.getNextValue();
        osc2Sample2 *= smoothedGain2.getNextValue();
        if (! stereoSwitch2)
            osc2Sample2 = osc2Sample;

        currentSample += osc2Sample;
        currentSample2 += osc2Sample2;

        for (auto i = outputBuffer.getNumChannels(); --i >= 0;) {
            if (i)
                outputBuffer.addSample(i, startSample, currentSample2);
            else
                outputBuffer.addSample(i, startSample, currentSample);
        }

        ++startSample;

        if (envAmp <= 0.0) {
            clearCurrentNote();
            break;
        }
    }
}

void MySynthesiserVoice::setEnvelopeParameters(ADSR::Parameters params) {
    envelope.setParameters(params);
}

void MySynthesiserVoice::setWavetypeParameter(int type1, int type2) {
    oscillator1Left.setWavetype(type1);
    oscillator1Right.setWavetype(type1);
    oscillator2Left.setWavetype(type2);
    oscillator2Right.setWavetype(type2);
}

void MySynthesiserVoice::setSharpParameter(float sharp1, float sharp2) {
    noteSharp1 = sharp1;
    noteSharp2 = sharp2;
}

void MySynthesiserVoice::setGainParameter(float gain1, float gain2) {
    smoothedGain1.setTargetValue(gain1);
    smoothedGain2.setTargetValue(gain2);
}

void MySynthesiserVoice::setStereoToggleParameter(int state1, int state2) {
    stereoSwitch1 = state1;
    stereoSwitch2 = state2;
}

void MySynthesiserVoice::setLFOWavetypeParameter(int type1, int type2) {
    lfo1.setWavetype(type1);
    lfo2.setWavetype(type2);
    if (type1 == 7)
        lfo1.setSharp(.75f);
    else
        lfo1.setSharp(1.f);
    if (type2 == 7)
        lfo2.setSharp(.75f);
    else
        lfo2.setSharp(1.f);
}

void MySynthesiserVoice::setLFOFreqParameter(float freq1, float freq2) {
    lfoFreq1 = freq1;
    lfoFreq2 = freq2;
    lfo1.setFreq(lfoFreq1);
    lfo2.setFreq(lfoFreq2);
}

void MySynthesiserVoice::setLFODepthParameter(float depth1, float depth2) {
    lfoDepth1 = depth1;
    lfoDepth2 = depth2;
}

void MySynthesiserVoice::setLFO1RoutingParameters(int lfo2freq, int lfo2freqInv, int lfo2depth, int lfo2depthInv,
                                                  int freq1, int freq1Inv, int sharp1, int sharp1Inv, int gain1, int gain1Inv,
                                                  int freq2, int freq2Inv, int sharp2, int sharp2Inv, int gain2, int gain2Inv) {
    lfo1lfo2freqSwith = lfo2freq; lfo1lfo2freqSwithInv = lfo2freqInv; lfo1lfo2depthSwith = lfo2depth; lfo1lfo2depthSwithInv = lfo2depthInv;
    lfo1freqSwitch1 = freq1; lfo1freqSwitch1Inv = freq1Inv; lfo1sharpSwitch1 = sharp1; lfo1sharpSwitch1Inv = sharp1Inv;
    lfo1gainSwitch1 = gain1; lfo1gainSwitch1Inv = gain1Inv; lfo1freqSwitch2 = freq2; lfo1freqSwitch2Inv = freq2Inv;
    lfo1sharpSwitch2 = sharp2; lfo1sharpSwitch2Inv = sharp2Inv; lfo1gainSwitch2 = gain2; lfo1gainSwitch2Inv = gain2Inv;
}

void MySynthesiserVoice::setLFO2RoutingParameters(int freq1, int freq1Inv, int sharp1, int sharp1Inv, int gain1, int gain1Inv,
                                                  int freq2, int freq2Inv, int sharp2, int sharp2Inv, int gain2, int gain2Inv) {
    lfo2freqSwitch1 = freq1; lfo2freqSwitch1Inv = freq1Inv; lfo2sharpSwitch1 = sharp1; lfo2sharpSwitch1Inv = sharp1Inv;
    lfo2gainSwitch1 = gain1; lfo2gainSwitch1Inv = gain1Inv; lfo2freqSwitch2 = freq2; lfo2freqSwitch2Inv = freq2Inv;
    lfo2sharpSwitch2 = sharp2; lfo2sharpSwitch2Inv = sharp2Inv; lfo2gainSwitch2 = gain2; lfo2gainSwitch2Inv = gain2Inv;
}

//==============================================================================
void MySynthesiser::setEnvelopeParameters(ADSR::Parameters params) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setEnvelopeParameters(params);
}

void MySynthesiser::setWavetypeParameter(int type1, int type2) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setWavetypeParameter(type1, type2);
}

void MySynthesiser::setSharpParameter(float sharp1, float sharp2) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setSharpParameter(sharp1, sharp2);
}

void MySynthesiser::setGainParameter(float gain1, float gain2) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setGainParameter(gain1, gain2);
}

void MySynthesiser::setStereoToggleParameter(int state1, int state2) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setStereoToggleParameter(state1, state2);
}

void MySynthesiser::setLFOWavetypeParameter(int type1, int type2) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setLFOWavetypeParameter(type1, type2);
}

void MySynthesiser::setLFOFreqParameter(float freq1, float freq2) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setLFOFreqParameter(freq1, freq2);
}

void MySynthesiser::setLFODepthParameter(float depth1, float depth2) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setLFODepthParameter(depth1, depth2);
}

void MySynthesiser::setLFO1RoutingParameters(int lfo2freq, int lfo2freqInv, int lfo2depth, int lfo2depthInv,
                                             int freq1, int freq1Inv, int sharp1, int sharp1Inv, int gain1, int gain1Inv,
                                             int freq2, int freq2Inv, int sharp2, int sharp2Inv, int gain2, int gain2Inv) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setLFO1RoutingParameters(lfo2freq, lfo2freqInv, lfo2depth, lfo2depthInv,
                                                                                  freq1, freq1Inv, sharp1, sharp1Inv, gain1, gain1Inv,
                                                                                  freq2, freq2Inv, sharp2, sharp2Inv, gain2, gain2Inv);
}

void MySynthesiser::setLFO2RoutingParameters(int freq1, int freq1Inv, int sharp1, int sharp1Inv, int gain1, int gain1Inv,
                                             int freq2, int freq2Inv, int sharp2, int sharp2Inv, int gain2, int gain2Inv) {
    for (int i = 0; i < getNumVoices(); i++)
       dynamic_cast<MySynthesiserVoice *> (getVoice(i))->setLFO2RoutingParameters(freq1, freq1Inv, sharp1, sharp1Inv, gain1, gain1Inv,
                                                                                  freq2, freq2Inv, sharp2, sharp2Inv, gain2, gain2Inv);
}

//==============================================================================
static String freqSliderValueToText(float value) {
    return String(value, 3) + String(" Hz");
}

static float freqSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String secondSliderValueToText(float value) {
    return String(value, 3) + String(" sec");
}

static float secondSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String sharpSliderValueToText(float value) {
    return String(value, 4) + String(" x");
}

static float sharpSliderTextToValue(const String& text) {
    return text.getFloatValue();
}

static String gainSliderValueToText(float value) {
    float val = 20.0f * log10f(jmax(0.001f, value));
    return String(val, 2) + String(" dB");
}

static float gainSliderTextToValue(const String& text) {
    float val = jlimit(-60.0f, 18.0f, text.getFloatValue());
    return powf(10.0f, val * 0.05f);
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    using Parameter = AudioProcessorValueTreeState::Parameter;

    std::vector<std::unique_ptr<Parameter>> parameters;

    parameters.push_back(std::make_unique<Parameter>(String("attack"), String("Attack"), String(),
                                                     NormalisableRange<float>(0.001f, 1.f, 0.001f, 0.5f),
                                                     0.005f, secondSliderValueToText, secondSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("decay"), String("Decay"), String(),
                                                     NormalisableRange<float>(0.001f, 1.f, 0.001f, 0.5f),
                                                     0.2f, secondSliderValueToText, secondSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("sustain"), String("Sustain"), String(),
                                                     NormalisableRange<float>(0.001f, 1.f, 0.001f, 0.3f),
                                                     0.708f, gainSliderValueToText, gainSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("release"), String("Release"), String(),
                                                     NormalisableRange<float>(0.001f, 1.f, 0.001f, 0.5f),
                                                     0.25f, secondSliderValueToText, secondSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1type"), String("LFO1Type"), String(),
                                                     NormalisableRange<float>(0.0f, 7.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1freq"), String("LFO1Freq"), String(),
                                                     NormalisableRange<float>(0.01f, 16.f, 0.001f, 0.3f),
                                                     0.5f, freqSliderValueToText, freqSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1depth"), String("LFO1Depth"), String(),
                                                     NormalisableRange<float>(0.f, 1.f, 0.001f, 1.f),
                                                     0.1f, sharpSliderValueToText, sharpSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteLfo2Freq"), String("LFO1RouteLfo2Freq"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteLfo2FreqInv"), String("LFO1RouteLfo2FreqInv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteLfo2Depth"), String("LFO1RouteLfo2Depth"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteLfo2DepthInv"), String("LFO1RouteLfo2DepthInv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteFreq1"), String("LFO1RouteFreq1"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteFreq1Inv"), String("LFO1RouteFreq1Inv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteSharp1"), String("LFO1RouteSharp1"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteSharp1Inv"), String("LFO1RouteSharp1Inv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteGain1"), String("LFO1RouteGain1"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteGain1Inv"), String("LFO1RouteGain1Inv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteFreq2"), String("LFO1RouteFreq2"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteFreq2Inv"), String("LFO1RouteFreq2Inv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteSharp2"), String("LFO1RouteSharp2"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteSharp2Inv"), String("LFO1RouteSharp2Inv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteGain2"), String("LFO1RouteGain2"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo1RouteGain2Inv"), String("LFO1RouteGain2Inv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo2type"), String("LFO2Type"), String(),
                                                     NormalisableRange<float>(0.0f, 7.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo2freq"), String("LFO2Freq"), String(),
                                                     NormalisableRange<float>(0.01f, 16.f, 0.001f, 0.3f),
                                                     0.5f, freqSliderValueToText, freqSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("lfo2depth"), String("LFO2Depth"), String(),
                                                     NormalisableRange<float>(0.f, 1.f, 0.001f, 1.f),
                                                     0.1f, sharpSliderValueToText, sharpSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("lfo2RouteFreq1"), String("LFO2RouteFreq1"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo2RouteFreq1Inv"), String("LFO2RouteFreq1Inv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo2RouteSharp1"), String("LFO2RouteSharp1"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo2RouteSharp1Inv"), String("LFO2RouteSharp1Inv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo2RouteGain1"), String("LFO2RouteGain1"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo2RouteGain1Inv"), String("LFO2RouteGain1Inv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo2RouteFreq2"), String("LFO2RouteFreq2"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo2RouteFreq2Inv"), String("LFO2RouteFreq2Inv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo2RouteSharp2"), String("LFO2RouteSharp2"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo2RouteSharp2Inv"), String("LFO2RouteSharp2Inv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo2RouteGain2"), String("LFO2RouteGain2"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("lfo2RouteGain2Inv"), String("LFO2RouteGain2Inv"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("type1"), String("Type1"), String(),
                                                     NormalisableRange<float>(0.0f, 7.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("sharp1"), String("Sharp1"), String(),
                                                     NormalisableRange<float>(0.f, 1.f, 0.001f, 0.5f),
                                                     0.5f, sharpSliderValueToText, sharpSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("gain1"), String("Gain1"), String(),
                                                     NormalisableRange<float>(0.001f, 7.94f, 0.001f, 0.3f),
                                                     1.0f, gainSliderValueToText, gainSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("stereo1"), String("Stereo1"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("type2"), String("Type2"), String(),
                                                     NormalisableRange<float>(0.0f, 7.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    parameters.push_back(std::make_unique<Parameter>(String("sharp2"), String("Sharp2"), String(),
                                                     NormalisableRange<float>(0.f, 1.f, 0.001f, 0.5f),
                                                     0.5f, sharpSliderValueToText, sharpSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("gain2"), String("Gain2"), String(),
                                                     NormalisableRange<float>(0.001f, 7.94f, 0.001f, 0.3f),
                                                     1.0f, gainSliderValueToText, gainSliderTextToValue));

    parameters.push_back(std::make_unique<Parameter>(String("stereo2"), String("Stereo2"), String(),
                                                     NormalisableRange<float>(0.0f, 1.0f, 1.f, 1.0f),
                                                     0.0f, nullptr, nullptr));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
Plugex_40_twoOscMidiSynthAudioProcessor::Plugex_40_twoOscMidiSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    parameters (*this, nullptr, Identifier(JucePlugin_Name), createParameterLayout())
{

    for (auto i = 0; i < numberOfVoices; ++i)
        synthesiser.addVoice(new MySynthesiserVoice());

    synthesiser.addSound(new MySynthesiserSound());

    attackParameter = parameters.getRawParameterValue("attack");
    decayParameter = parameters.getRawParameterValue("decay");
    sustainParameter = parameters.getRawParameterValue("sustain");
    releaseParameter = parameters.getRawParameterValue("release");

    lfo1typeParameter = parameters.getRawParameterValue("lfo1type");
    lfo1freqParameter = parameters.getRawParameterValue("lfo1freq");
    lfo1depthParameter = parameters.getRawParameterValue("lfo1depth");
    lfo1RouteLfo2FreqParameter = parameters.getRawParameterValue("lfo1RouteLfo2Freq");
    lfo1RouteLfo2FreqInvParameter = parameters.getRawParameterValue("lfo1RouteLfo2FreqInv");
    lfo1RouteLfo2DepthParameter = parameters.getRawParameterValue("lfo1RouteLfo2Depth");
    lfo1RouteLfo2DepthInvParameter = parameters.getRawParameterValue("lfo1RouteLfo2DepthInv");
    lfo1RouteFreq1Parameter = parameters.getRawParameterValue("lfo1RouteFreq1");
    lfo1RouteFreq1InvParameter = parameters.getRawParameterValue("lfo1RouteFreq1Inv");
    lfo1RouteSharp1Parameter = parameters.getRawParameterValue("lfo1RouteSharp1");
    lfo1RouteSharp1InvParameter = parameters.getRawParameterValue("lfo1RouteSharp1Inv");
    lfo1RouteGain1Parameter = parameters.getRawParameterValue("lfo1RouteGain1");
    lfo1RouteGain1InvParameter = parameters.getRawParameterValue("lfo1RouteGain1Inv");
    lfo1RouteFreq2Parameter = parameters.getRawParameterValue("lfo1RouteFreq2");
    lfo1RouteFreq2InvParameter = parameters.getRawParameterValue("lfo1RouteFreq2Inv");
    lfo1RouteSharp2Parameter = parameters.getRawParameterValue("lfo1RouteSharp2");
    lfo1RouteSharp2InvParameter = parameters.getRawParameterValue("lfo1RouteSharp2Inv");
    lfo1RouteGain2Parameter = parameters.getRawParameterValue("lfo1RouteGain2");
    lfo1RouteGain2InvParameter = parameters.getRawParameterValue("lfo1RouteGain2Inv");

    lfo2typeParameter = parameters.getRawParameterValue("lfo2type");
    lfo2freqParameter = parameters.getRawParameterValue("lfo2freq");
    lfo2depthParameter = parameters.getRawParameterValue("lfo2depth");
    lfo2RouteFreq1Parameter = parameters.getRawParameterValue("lfo2RouteFreq1");
    lfo2RouteFreq1InvParameter = parameters.getRawParameterValue("lfo2RouteFreq1Inv");
    lfo2RouteSharp1Parameter = parameters.getRawParameterValue("lfo2RouteSharp1");
    lfo2RouteSharp1InvParameter = parameters.getRawParameterValue("lfo2RouteSharp1Inv");
    lfo2RouteGain1Parameter = parameters.getRawParameterValue("lfo2RouteGain1");
    lfo2RouteGain1InvParameter = parameters.getRawParameterValue("lfo2RouteGain1Inv");
    lfo2RouteFreq2Parameter = parameters.getRawParameterValue("lfo2RouteFreq2");
    lfo2RouteFreq2InvParameter = parameters.getRawParameterValue("lfo2RouteFreq2Inv");
    lfo2RouteSharp2Parameter = parameters.getRawParameterValue("lfo2RouteSharp2");
    lfo2RouteSharp2InvParameter = parameters.getRawParameterValue("lfo2RouteSharp2Inv");
    lfo2RouteGain2Parameter = parameters.getRawParameterValue("lfo2RouteGain2");
    lfo2RouteGain2InvParameter = parameters.getRawParameterValue("lfo2RouteGain2Inv");

    type1Parameter = parameters.getRawParameterValue("type1");
    sharp1Parameter = parameters.getRawParameterValue("sharp1");
    gain1Parameter = parameters.getRawParameterValue("gain1");
    stereo1Parameter = parameters.getRawParameterValue("stereo1");

    type2Parameter = parameters.getRawParameterValue("type2");
    sharp2Parameter = parameters.getRawParameterValue("sharp2");
    gain2Parameter = parameters.getRawParameterValue("gain2");
    stereo2Parameter = parameters.getRawParameterValue("stereo2");
}

Plugex_40_twoOscMidiSynthAudioProcessor::~Plugex_40_twoOscMidiSynthAudioProcessor()
{
}

//==============================================================================
const String Plugex_40_twoOscMidiSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugex_40_twoOscMidiSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_40_twoOscMidiSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugex_40_twoOscMidiSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugex_40_twoOscMidiSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugex_40_twoOscMidiSynthAudioProcessor::getNumPrograms()
{
return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugex_40_twoOscMidiSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugex_40_twoOscMidiSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugex_40_twoOscMidiSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugex_40_twoOscMidiSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugex_40_twoOscMidiSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    keyboardState.reset();
    synthesiser.setCurrentPlaybackSampleRate(sampleRate);
}

void Plugex_40_twoOscMidiSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugex_40_twoOscMidiSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Plugex_40_twoOscMidiSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    synthesiser.setEnvelopeParameters(ADSR::Parameters {*attackParameter, *decayParameter, *sustainParameter, *releaseParameter});
    synthesiser.setWavetypeParameter((int)*type1Parameter, (int)*type2Parameter);
    synthesiser.setSharpParameter(*sharp1Parameter, *sharp2Parameter);
    synthesiser.setGainParameter(*gain1Parameter, *gain2Parameter);
    synthesiser.setStereoToggleParameter((int)*stereo1Parameter, (int)*stereo2Parameter);
    synthesiser.setLFOWavetypeParameter((int)*lfo1typeParameter, (int)*lfo2typeParameter);
    synthesiser.setLFOFreqParameter(*lfo1freqParameter, *lfo2freqParameter);
    synthesiser.setLFODepthParameter(*lfo1depthParameter, *lfo2depthParameter);
    synthesiser.setLFO1RoutingParameters(*lfo1RouteLfo2FreqParameter, *lfo1RouteLfo2FreqInvParameter,
                                         *lfo1RouteLfo2DepthParameter, *lfo1RouteLfo2DepthInvParameter,
                                         *lfo1RouteFreq1Parameter, *lfo1RouteFreq1InvParameter,
                                         *lfo1RouteSharp1Parameter, *lfo1RouteSharp1InvParameter,
                                         *lfo1RouteGain1Parameter, *lfo1RouteGain1InvParameter,
                                         *lfo1RouteFreq2Parameter, *lfo1RouteFreq2InvParameter,
                                         *lfo1RouteSharp2Parameter, *lfo1RouteSharp2InvParameter,
                                         *lfo1RouteGain2Parameter, *lfo1RouteGain2InvParameter);
    synthesiser.setLFO2RoutingParameters(*lfo2RouteFreq1Parameter, *lfo2RouteFreq1InvParameter,
                                         *lfo2RouteSharp1Parameter, *lfo2RouteSharp1InvParameter,
                                         *lfo2RouteGain1Parameter, *lfo2RouteGain1InvParameter,
                                         *lfo2RouteFreq2Parameter, *lfo2RouteFreq2InvParameter,
                                         *lfo2RouteSharp2Parameter, *lfo2RouteSharp2InvParameter,
                                         *lfo2RouteGain2Parameter, *lfo2RouteGain2InvParameter);

    synthesiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool Plugex_40_twoOscMidiSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugex_40_twoOscMidiSynthAudioProcessor::createEditor()
{
    return new Plugex_40_twoOscMidiSynthAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Plugex_40_twoOscMidiSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugex_40_twoOscMidiSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugex_40_twoOscMidiSynthAudioProcessor();
}
