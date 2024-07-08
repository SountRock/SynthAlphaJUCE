/*
  ==============================================================================

    WTSynth.h
    Created: 23 Aug 2023 1:39:34pm
    Author:  LKDat

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ADSRData.h"

class WTSynth : public juce::Synthesiser
{
public:
	WTSynth();
	~WTSynth();
	void prepareToPlay(double sampleRate);
	void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages, float volume);
	void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample, float volume); //

private:
	void initializeOscillators();
	std::vector<float> generateSineWaveTable();
	void handleMidiEvent(const juce::MidiMessage& midiEvent, juce::AudioBuffer<float>& buffer);
	float midiNoteNumberToFrequency(int midiNoteNumber);
	//

	double sampleRate;
	std::vector<WTOSC> OSCillators;
};

class WTSound : public juce::SynthesiserSound {
public:
	WTSound::WTSound() {}
	WTSound::~WTSound() {}
	bool WTSound::appliesToNote(int midiNoteNumber) override {
		//Возвращает значение true, если этот звук должен воспроизводиться при нажатии данной midi-ноты.
		return true;
	};

	bool WTSound::appliesToChannel(int midiChannel) override {
		//Возвращает значение true, если звук должен быть вызван midi-событиями на данном канале.
		return true;
	};
};

class WTVoice : public juce::SynthesiserVoice {
public:
	WTVoice::WTVoice() {}
	WTVoice::~WTVoice() {}
	bool canPlaySound(juce::SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
	void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChanals);
	void releaseResources();
	void update(const float attack, const float decay, const float sustain, const float realeaze);

	WTSynth& getOscillator1() { return synth1; };
	WTSynth& getOscillator2() { return synth2; };
	WTSynth& getOscillator3() { return synth3; };
	WTSynth& getOscillator4() { return synth3; };
private:
	ADSRData adsr;
	WTSynth synth1;
	WTSynth synth2;
	WTSynth synth3;
	WTSynth synth4;
	bool isPrepared{ false };
	juce::Array<juce::AudioBuffer<float>> synthBuffer;
};

