/*
  ==============================================================================

    OSCData.h
    Created: 18 Mar 2023 8:00:06pm
    Author:  LKDat

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OSCData : public juce::dsp::Oscillator<float> {
public:
	void prepareToPlay(juce::dsp::ProcessSpec& spec);
	void getNextAudioBlock(juce::dsp::AudioBlock<float> block);
	void setWaveFrequency(const int midiNoteNumber);
	void setWaveType(const int choice);
	void setOSCParams(const int status, const float freq, const float freqFM, const float depth);
	//void setFMOscillator(OSCData& osc);
	Array<float> getWave();
	void setModWave(Array<float> modWave);
	void setWave(Array<float> wave);
	void resetRecource();
	void setWaveSample(float value);

private:
	//std::vector<std::vector<float>> wave;
	//std::vector<std::vector<float>> modWaveOn;
	////////////////////////////////////////
	Array<float> WT;
	
//	Array<float> WTSqu;
//	Array<float> WTSaw;
	Array<float> mod;
	int wtSize{ 1024 };
	double phase{ 0.0 };
	float increment{ 0.0f };
	double sampleWave{ 0.0f };
//	int choiseWT { 0 };
	////////////////////////////////////////
	//juce::Array<juce::Array<float>> waveBank;
	//juce::Array<NumericType> sinWaveSample;
	//juce::dsp::Oscillator<float> fmOsc{ [](float x) {return std::sin(x); } };
	juce::dsp::Oscillator<float> fmOsc;
	juce::dsp::Gain<float> gain;
	//float volume = 60;
	int LastMidiNote { 0 };
	float fmMod { 0.0f };
	float fmDepth{ 0.0f };

};
