/*
  ==============================================================================

    WTSynth.cpp
    Created: 23 Aug 2023 1:39:34pm
    Author:  LKDat

  ==============================================================================
*/

/*
  ==============================================================================

    WT.cpp
    Created: 16 Aug 2023 2:31:13pm
    Author:  LKDat

  ==============================================================================
*/

#include "WTSynth.h"

//=============================================================================================================================================================
//SYNTH
WTSynth::WTSynth()
{
}

WTSynth::~WTSynth()
{
}

void WTSynth::prepareToPlay(double sampleRate)
{
	this->sampleRate = sampleRate;

	initializeOscillators();
}

void WTSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages, float volume)
{
	auto currentSample = 0;

	for (const auto midiMessage : midiMessages) {
		const auto midiEvent = midiMessage.getMessage();
		const auto midiEventSample = static_cast<int>(midiEvent.getTimeStamp());

		render(buffer, currentSample, midiEventSample, volume);
		handleMidiEvent(midiEvent, buffer);

		currentSample = midiEventSample;
	}

	render(buffer, currentSample, buffer.getNumSamples(), volume);
}


void WTSynth::initializeOscillators()
{
	constexpr auto OSCILLATORS_COUNT = 128;

	const auto waveTable = generateSineWaveTable();

	OSCillators.clear();

	for (auto i = 0; i < OSCILLATORS_COUNT; ++i) {
		OSCillators.emplace_back(waveTable, sampleRate);
	}
}

void WTSynth::handleMidiEvent(const juce::MidiMessage& midiEvent, juce::AudioBuffer<float>& buffer)
{
	if (midiEvent.isNoteOn()) {
		const auto oscillatorID = midiEvent.getNoteNumber();
		const auto freq = midiNoteNumberToFrequency(oscillatorID);
		OSCillators[oscillatorID].setFrequency(freq);
	}
	else if (midiEvent.isNoteOff()) {
		const auto oscillatorID = midiEvent.getNoteNumber();
		OSCillators[oscillatorID].stop();
	}
	else if (midiEvent.isAllNotesOff()) {
		for (auto& oscillator : OSCillators) {
			oscillator.stop();
		}
	}
}

float WTSynth::midiNoteNumberToFrequency(int midiNoteNumber)
{
	constexpr auto A4_FREQUENCY = 440.f;
	constexpr auto A4_NOTE_NUMBER = 69.f;
	constexpr auto SEMITONES_IN_AN_OCTAVE = 12.f;

	return A4_FREQUENCY * std::powf(2.f, (midiNoteNumber - A4_NOTE_NUMBER) / SEMITONES_IN_AN_OCTAVE);
}

void WTSynth::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample, float volume)
{
	auto* firstChannel = buffer.getWritePointer(0);

	for (auto& oscillator : OSCillators) {
		if (oscillator.isPlaying()) {
			for (auto sample = startSample; sample < endSample; ++sample) {
				firstChannel[sample] += oscillator.getSample() * volume;
			}
		}
	}

	for (auto channel = 1; channel < buffer.getNumChannels(); ++channel) {
		std::copy(firstChannel + startSample, firstChannel + endSample, buffer.getWritePointer(channel) + startSample);
	}
}

std::vector<float> WTSynth::generateSineWaveTable()
{
	constexpr auto WAVETABLE_LENGTH = 64;

	std::vector<float> sineWaveTable(WAVETABLE_LENGTH);

	const auto PI = std::atanf(1.f) * 4;

	for (auto i = 0; i < WAVETABLE_LENGTH; ++i) {
		sineWaveTable[i] = std::sinf(2 * PI * static_cast<float>(i) / static_cast<float>(WAVETABLE_LENGTH));
	}

	return sineWaveTable;
}
//SYNTH
//=============================================================================================================================================================

//=============================================================================================================================================================
//VOICE
bool WTVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void WTVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
	adsr.noteOn();
	DBG("OnNote");
}

void WTVoice::stopNote(float velocity, bool allowTailOff)
{
	adsr.noteOff();
	if (!allowTailOff || !adsr.isActive())
		clearCurrentNote();
}

void WTVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChanals)
{
	synth1.prepareToPlay(sampleRate);
	synth2.prepareToPlay(sampleRate);
	synth3.prepareToPlay(sampleRate);
	synth4.prepareToPlay(sampleRate);
	adsr.setSampleRate(sampleRate);

	isPrepared = true;
}

void WTVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	jassert(isPrepared);

	if (!isVoiceActive())
		return;

	synthBuffer[0].setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	synthBuffer[0].clear();

	synth1.render(synthBuffer[0], startSample, numSamples, 1.0f);

	adsr.applyEnvelopeToBuffer(outputBuffer, 0, outputBuffer.getNumSamples());

	for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++) {
		outputBuffer.addFrom(channel, startSample, synthBuffer[0], channel, 0, numSamples);
		if (!adsr.isActive())
			clearCurrentNote();
	}
}

void WTVoice::releaseResources()
{
	adsr.reset();
}

void WTVoice::update(const float attack, const float decay, const float sustain, const float realeaze)
{
	adsr.updateADSR(attack, decay, sustain, realeaze);
}
//VOICE
//=============================================================================================================================================================

