/*
  ==============================================================================

    SynthVoice.cpp
    Created: 24 Feb 2023 3:44:29pm
    Author:  LKDat

  ==============================================================================
*/

#include "SynthVoice.h"


bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {

//	Äîëæåí âîçâðàùàòü çíà÷åíèå true, åñëè ýòîò ãîëîñîâîé îáúåêò ñïîñîáåí âîñïðîèçâîäèòü äàííûé çâóê.
//	Åñëè ñóùåñòâóþò ðàçíûå êëàññû çâóêà è ðàçíûå êëàññû ãîëîñà, ãîëîñ ìîæåò âûáðàòü, êàêèå èç íèõ îí õî÷åò èñïîëüçîâàòü.
//	Òèïè÷íàÿ ðåàëèçàöèÿ ýòîãî ìåòîäà ìîæåò ïðîñòî âîçâðàùàòü çíà÷åíèå true, åñëè ñóùåñòâóåò òîëüêî îäèí òèï ãîëîñà è çâóêà,
//	èëè îí ìîæåò ïðîâåðèòü òèï ïåðåäàííîãî îáúåêòà sound è ïîñìîòðåòü, òîò ëè ýòî, êîòîðûé îí ïîíèìàåò.


	//åñëè ïåðåäà÷à çâóêà ÷åðåç êëàññ SynthSound óäàëàñòü òî...
return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
//
}
//==============================================================================

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
	//Âûçûâàåòñÿ ïðè âîñïðîèçâåäåíèè íîâîé íîòû
	//frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
	//DBG(midiNoteNumber);

	osc.setWaveFrequency(midiNoteNumber);
	osc2.setWaveFrequency(midiNoteNumber);
	osc3.setWaveFrequency(midiNoteNumber);
	osc4.setWaveFrequency(midiNoteNumber);

	//oscArray[0].setWaveFrequency(midiNoteNumber);
	//oscArray[1].setWaveFrequency(midiNoteNumber);
	//oscArray[2].setWaveFrequency(midiNoteNumber);
	//oscArray[3].setWaveFrequency(midiNoteNumber);
	//oscArray[4].setWaveFrequency(midiNoteNumber);
	//oscArray[5].setWaveFrequency(midiNoteNumber);

	adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff) {
	adsr.noteOff();

	if (!allowTailOff || !adsr.isActive()) 
		clearCurrentNote();

}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {
	//Âûçûâàåòñÿ, ÷òîáû ñîîáùèòü ãîëîñó, ÷òî Pitch Wheel áûëî ïåðåìåùåíî.
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {
	//Âûçûâàåòñÿ, ÷òîáû ñîîáùèòü ãîëîñó, ÷òî midi - êîíòðîëëåð áûë ïåðåìåùåí.
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChanals)
{
	juce::dsp::ProcessSpec spec{ sampleRate, samplesPerBlock, outputChanals };
	
	osc.prepareToPlay(spec);
	osc2.prepareToPlay(spec);
	osc3.prepareToPlay(spec);
	osc4.prepareToPlay(spec);

	//oscArray[0].prepareToPlay(spec);
	//oscArray[1].prepareToPlay(spec);
	//oscArray[2].prepareToPlay(spec);
	//oscArray[3].prepareToPlay(spec);
	//oscArray[4].prepareToPlay(spec);
	//oscArray[5].prepareToPlay(spec);

	adsr.setSampleRate(sampleRate);
	//filter.prepareToPlay(sampleRate, samplesPerBlock, outputChanals); //
	//modADSR.setSampleRate(sampleRate); //
	gain.prepare(spec);

	//osc.setFrequency(2000.f);
	//osc2.setFrequency(220.f);
	//osc3.setFrequency(220.f);
	//osc4.setFrequency(220.f);
	//osc5.setFrequency(220.f);
	//osc6.setFrequency(220.f);

	isPrepared = true;

	
}

void SynthVoice::releaseResources()
{
	osc.resetRecource();
	osc2.resetRecource();
	osc3.resetRecource();
	osc4.resetRecource();

	//oscArray[0].reset();
	//oscArray[1].reset();
	//oscArray[2].reset();
	//oscArray[3].reset();
	//oscArray[4].reset();
	//oscArray[5].reset();
	gain.reset();
	adsr.reset();
}

void SynthVoice::update(const float attack, const float decay, const float sustain, const float realeaze, const float volume1, const float volume2, const float volume3, const float volume4, const float volume)
{
	adsr.updateADSR(attack, decay, sustain, realeaze);
	float allVolume = volume < 60 ? volume / 100 : (volume - 40) / 100;
	volumeOSC = (volume1 / 100) * allVolume;
	volumeOSC2 = (volume2 / 100) * allVolume;
	volumeOSC3 = (volume3 / 100) * allVolume;
	volumeOSC4 = (volume4 / 100) * allVolume;
	gain.setGainLinear(allVolume);
}

/*
void SynthVoice::setOSCFrequency(const float freq1, const float freq2, const float freq3, const float freq4, const float freq5, const float freq6)
{
	this->freq1 = freq1;
	this->freq2 = freq2;
	this->freq3 = freq3;
	this->freq4 = freq4;
	this->freq5 = freq5;
	this->freq6 = freq6;
}
*/


void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
	//Îòîáðàæàåò ñëåäóþùèé áëîê äàííûõ äëÿ ýòîãî ãîëîñà.
	jassert(isPrepared);

	if (!isVoiceActive())
		return;


	synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	synthBuffer.clear();

	//synthBufferArray[0].setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	//synthBufferArray[0].clear();

	//////////////////////OSC buffer
	synthBuffer1.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	synthBuffer1.clear();

	synthBuffer2.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	synthBuffer2.clear();

	synthBuffer3.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	synthBuffer3.clear();

	synthBuffer4.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	synthBuffer4.clear();

	//synthBufferArray[1].setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	//synthBufferArray[1].clear();

	//synthBufferArray[2].setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	//synthBufferArray[2].clear();

	//synthBufferArray[3].setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	//synthBufferArray[3].clear();

	//synthBufferArray[4].setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	//synthBufferArray[4].clear();

	//synthBufferArray[5].setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	//synthBufferArray[5].clear();

	//synthBufferArray[6].setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	//synthBufferArray[6].clear();
	//////////////////////
	
		//////////////////////OSC Load
		//std::vector<juce::dsp::AudioBlock<float>> audioBlockArray;
		juce::dsp::AudioBlock<float> audioBlock1{ synthBuffer1 };
		juce::dsp::AudioBlock<float> audioBlock2{ synthBuffer2 };
		juce::dsp::AudioBlock<float> audioBlock3{ synthBuffer3 };
		juce::dsp::AudioBlock<float> audioBlock4{ synthBuffer4 };
		osc.getNextAudioBlock(audioBlock1);
		osc2.getNextAudioBlock(audioBlock2);
		osc3.getNextAudioBlock(audioBlock3);
		osc4.getNextAudioBlock(audioBlock4);
		/*
		audioBlockArray[1] = synthBufferArray[1];
		audioBlockArray[2] = synthBufferArray[2];
		audioBlockArray[3] = synthBufferArray[3];
		audioBlockArray[4] = synthBufferArray[4];
		audioBlockArray[5] = synthBufferArray[5];
		audioBlockArray[6] = synthBufferArray[6];

		oscArray[0].getNextAudioBlock(audioBlockArray[1]);
		oscArray[1].getNextAudioBlock(audioBlockArray[2]);
		oscArray[2].getNextAudioBlock(audioBlockArray[3]);
		oscArray[3].getNextAudioBlock(audioBlockArray[4]);
		oscArray[4].getNextAudioBlock(audioBlockArray[5]);
		oscArray[5].getNextAudioBlock(audioBlockArray[6]);
		*/
		//////////////////////

		//////////////////////Buffer fusion
		for (int ch = 0; ch < outputBuffer.getNumChannels(); ch++) {
			auto channelData = synthBuffer.getWritePointer(ch);
			auto channelDataOSC1 = synthBuffer1.getWritePointer(ch);
			auto channelDataOSC2 = synthBuffer2.getWritePointer(ch);
			auto channelDataOSC3 = synthBuffer3.getWritePointer(ch);
			auto channelDataOSC4 = synthBuffer4.getWritePointer(ch);
			for (int s = 0; s < outputBuffer.getNumSamples(); s++) {
				channelDataOSC1[s] = synthBuffer1.getSample(ch, s) * (volumeOSC);
				channelDataOSC2[s] = synthBuffer2.getSample(ch, s) * (volumeOSC2);
				channelDataOSC3[s] = synthBuffer3.getSample(ch, s) * (volumeOSC3);
				channelDataOSC4[s] = synthBuffer4.getSample(ch, s) * (volumeOSC4);
				channelData[s] = channelDataOSC1[s] + channelDataOSC2[s] + channelDataOSC3[s] + channelDataOSC3[s] + channelDataOSC4[s];
			}
		}
		/*
		for (int ch = 0; ch < outputBuffer.getNumChannels(); ch++) {
			auto channelData = synthBufferArray[0].getWritePointer(ch);
			auto channelDataOSC1 = synthBufferArray[1].getWritePointer(ch);
			auto channelDataOSC2 = synthBufferArray[2].getWritePointer(ch);
			auto channelDataOSC3 = synthBufferArray[3].getWritePointer(ch);
			auto channelDataOSC4 = synthBufferArray[4].getWritePointer(ch);
			auto channelDataOSC5 = synthBufferArray[5].getWritePointer(ch);
			auto channelDataOSC6 = synthBufferArray[6].getWritePointer(ch);
			for (int s = 0; s < outputBuffer.getNumSamples(); s++) {
				channelDataOSC1[s] = synthBufferArray[1].getSample(ch, s) * (volumeOSC < 60 ? volumeOSC / 100 : (volumeOSC - 40) / 100);
				channelDataOSC2[s] = synthBufferArray[2].getSample(ch, s) * (volumeOSC2 < 60 ? volumeOSC2 / 100 : (volumeOSC2 - 40) / 100);
				channelDataOSC3[s] = synthBufferArray[3].getSample(ch, s) * (volumeOSC3 < 60 ? volumeOSC3 / 100 : (volumeOSC3 - 40) / 100);
				channelDataOSC4[s] = synthBufferArray[4].getSample(ch, s) * (volumeOSC4 < 60 ? volumeOSC4 / 100 : (volumeOSC4 - 40) / 100);
				channelDataOSC5[s] = synthBufferArray[5].getSample(ch, s) * (volumeOSC5 < 60 ? volumeOSC5 / 100 : (volumeOSC5 - 40) / 100);
				channelDataOSC6[s] = synthBufferArray[6].getSample(ch, s) * (volumeOSC6 < 60 ? volumeOSC6 / 100 : (volumeOSC6 - 40) / 100);
				channelData[s] = channelDataOSC1[s] + channelDataOSC2[s] + channelDataOSC3[s] + channelDataOSC3[s] + channelDataOSC4[s] + channelDataOSC5[s] + channelDataOSC6[s];
			}
		}
		*/
		//////////////////////
		juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
		gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
		adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());


		for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++) {
			outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
			if (!adsr.isActive())
				clearCurrentNote();
		}
		/*
		audioBlockArray[0] = synthBufferArray[0];
		gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlockArray[0]));
		adsr.applyEnvelopeToBuffer(synthBufferArray[0], 0, synthBufferArray[0].getNumSamples());


		for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++) {
			outputBuffer.addFrom(channel, startSample, synthBufferArray[0], channel, 0, numSamples);
			if (!adsr.isActive())
				clearCurrentNote();
		}
		*/
		///////////////////////////////
		//14:01
		///////////////////////////////
}



