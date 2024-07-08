/*
  ==============================================================================

    SynthVoice.h
    Created: 24 Feb 2023 1:12:19pm
    Author:  LKDat

  ==============================================================================
*/

#pragma once
using namespace std;
#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data//ADSRData.h"
#include "Data//OSCData.h"
#include "Data//FilterData.h"
#include <vector>

class SynthVoice : public juce::SynthesiserVoice {
public:
	SynthVoice::SynthVoice() {};
	SynthVoice::~SynthVoice() {};

	bool canPlaySound(juce::SynthesiserSound* sound) override;

	//==============================================================================

	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;

	void stopNote(float velocity, bool allowTailOff) override;

	void pitchWheelMoved(int newPitchWheelValue) override;

	void controllerMoved(int controllerNumber, int newControllerValue) override;

	void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

	void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChanals);

	void releaseResources();

	void update(const float attack, const float decay, const float sustain, const float realeaze, const float volume1, const float volume2, const float volume3, const float volume4, const float volume);

	/*
	void prepareOsc() {
		oscArray.resize(6);
		synthBufferArray.resize(6);
		oscArray[0].reset();
		oscArray[1].reset();
		oscArray[2].reset();
		oscArray[3].reset();
		oscArray[4].reset();
		oscArray[5].reset();

		synthBufferArray[0].clear();
		synthBufferArray[1].clear();
		synthBufferArray[2].clear();
		synthBufferArray[3].clear();
		synthBufferArray[4].clear();
		synthBufferArray[5].clear();
	};
	*/
	//////////////////////
	OSCData& getOscillator() { return osc; };
	OSCData& getOscillator2() { return osc2; };
	OSCData& getOscillator3() { return osc3; };
	OSCData& getOscillator4() { return osc4; };

private:
	///////////////////////////////
	Array<float> oscWS1;
	///////////////////////////////

	ADSRData adsr;

	//////////////////////
	OSCData osc;
	OSCData osc2;
	OSCData osc3;
	OSCData osc4;
	juce::AudioBuffer<float> synthBuffer;
	juce::AudioBuffer<float> synthBuffer1;
	juce::AudioBuffer<float> synthBuffer2;
	juce::AudioBuffer<float> synthBuffer3;
	juce::AudioBuffer<float> synthBuffer4;

	float volumeOSC = 100.0f;
	float volumeOSC2 = 100.0f;
	float volumeOSC3 = 100.0f;
	float volumeOSC4 = 100.0f;
	//////////////////////

	///////////////////////////////
	//FilterData filter;
	//ADSRData modADSR; 
	//////////////////////////////

	juce::dsp::Gain<float> gain;

	bool isPrepared{ false };

	//double level;
	double frequency;
	
};

