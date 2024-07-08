/*
  ==============================================================================

    SynthSound.h
    Created: 24 Feb 2023 1:11:44pm
    Author:  LKDat

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthVoice.h"

class SynthSound : public juce::SynthesiserSound{
public:
	SynthSound::SynthSound() {}
	SynthSound::~SynthSound() {}
	bool SynthSound::appliesToNote(int midiNoteNumber) override {
		//Возвращает значение true, если этот звук должен воспроизводиться при нажатии данной midi-ноты.
		return true;
	};

	bool SynthSound::appliesToChannel(int midiChannel) override {
		//Возвращает значение true, если звук должен быть вызван midi-событиями на данном канале.
		return true;
	};
};