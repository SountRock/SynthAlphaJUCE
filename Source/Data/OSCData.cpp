/*
  ==============================================================================

    OSCData.cpp
    Created: 18 Mar 2023 8:00:06pm
    Author:  LKDat

  ==============================================================================
*/

#include "OSCData.h"

void OSCData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
	fmOsc.prepare(spec);
	prepare(spec);
}

void OSCData::getNextAudioBlock(juce::dsp::AudioBlock<float> block)
{
	for (int ch = 0; ch < block.getNumChannels(); ch++) {
		for (int s = 0; s < block.getNumSamples(); s++) {
			fmMod = fmOsc.processSample(block.getSample(ch, s)) * fmDepth; //past var wave
		}
	}

	process(juce::dsp::ProcessContextReplacing<float>(block));
}

void OSCData::resetRecource()
{
	WT.clear();
	reset();
	fmOsc.reset();
}

void OSCData::setWaveSample(float value)
{
	initialise([value](int x) {return value; });
}

void OSCData::setWaveFrequency(const int midiNoteNumber)
{
	setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);

	LastMidiNote = midiNoteNumber;
}

void OSCData::setWaveType(const int choice)
{
	
	switch (choice) {
	case 0:
		//Sine
		initialise([](float x) {return (std::sin(x)); });
		break;

	case 1:
		//Saw
		initialise([](float x) {return x / MathConstants<float>::pi; });
		break;

	case 2:
		//Square
		initialise([](float x) {return (x < 0.0f ? -1.0f : 1.0f); });
		break;

	default:
		jassertfalse;
		break;
	}
	
	//initialise([this](int x) { return WT[x]; });
}

void OSCData::setOSCParams(const int status, const float freq, const float freqFM, const float depth)
{
	fmOsc.setFrequency(freqFM);
	fmDepth = depth;
	float plusFreq;
	if (status == 0) {
		plusFreq = freq + fmMod;
	}
	if (status == 1) {
		plusFreq = fmMod;
	}
	auto currentFreq = juce::MidiMessage::getMidiNoteInHertz(LastMidiNote) + plusFreq;
	setFrequency(currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
}

Array<float> OSCData::getWave()
{
	return WT;
}

void OSCData::setModWave(Array<float> modWave)
{
	fmOsc.initialise([modWave](int x) { return modWave[x]; });
}

void OSCData::setWave(Array<float> wave)
{
	WT.clear();
	WT.addArray(wave);
	initialise([wave](int x) {return wave[x]; });
}






