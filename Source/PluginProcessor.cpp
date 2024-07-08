/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAlphaAudioProcessor::SynthAlphaAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
	 ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
	/*
	for (int i = 0; i < synth.getNumVoices(); i++) {
		if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
			voice->getOscillator().setWaveType(0);
		}
	}
	*/
	synth.clearVoices();

	for (int i = 0; i < 8; i++) {
		synth.addVoice(new SynthVoice());
	}

	synth.clearSounds();
	synth.addSound(new SynthSound());

	for (float s = 0; s < 1024.f; s++) {
		WTSin.insert(s, std::sin(s));
		WTSaw.insert(s, s / MathConstants<float>::pi);
		WTSqu.insert(s, s < 0.0f ? -1.0f : 1.0f);
	}

}

SynthAlphaAudioProcessor::~SynthAlphaAudioProcessor()
{
}

//==============================================================================
const juce::String SynthAlphaAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthAlphaAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthAlphaAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthAlphaAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthAlphaAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthAlphaAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthAlphaAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthAlphaAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthAlphaAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthAlphaAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthAlphaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	//lastSampleRate = sampleRate; //!!
	synth.setCurrentPlaybackSampleRate(sampleRate);

	
	for (int i = 0; i < synth.getNumVoices(); i++) {
		//åñëè ïðåîáðàçîâàíèå óñïåøíî, òî
		if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
			voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
		}
	}

	//for (int) {}

	filter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());

}

void SynthAlphaAudioProcessor::releaseResources()
{
	for (int i = 0; i < synth.getNumVoices(); i++) {
		if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
			voice->releaseResources();
		}
	}
	filter.reset();

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthAlphaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void SynthAlphaAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
	
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	//voice->oscArray[0].reset();
	//voice->oscArray[1].reset();
	//voice->oscArray[2].reset();
	//voice->oscArray[3].reset();
	//voice->oscArray[4].reset();
	//voice->oscArray[5].reset();

	//voice->synthBufferArray[0].clear();
	//voice->synthBufferArray[1].clear();
	//voice->synthBufferArray[2].clear();
	//voice->synthBufferArray[3].clear();
	//voice->synthBufferArray[4].clear();
	//voice->synthBufferArray[5].clear();

	for (int i; i < synth.getNumVoices(); i++) {
		if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
				//Osc controls
				//ADSR

				//voice->prepareOsc();

				auto& attack = *apvts.getRawParameterValue("ATTACK");
				auto& decay = *apvts.getRawParameterValue("DECAY");
				auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
				auto& releaze = *apvts.getRawParameterValue("RELEAZE");

				auto& oscStatus1 = *apvts.getRawParameterValue("OSC1STATUS");
				auto& oscStatus2 = *apvts.getRawParameterValue("OSC2STATUS");
				auto& oscStatus3 = *apvts.getRawParameterValue("OSC3STATUS");
				auto& oscStatus4 = *apvts.getRawParameterValue("OSC4STATUS");

				auto& oscWaveChoice = *apvts.getRawParameterValue("OSC1WAVETYPE");
				auto& oscWaveChoice2 = *apvts.getRawParameterValue("OSC2WAVETYPE");
				auto& oscWaveChoice3 = *apvts.getRawParameterValue("OSC3WAVETYPE");
				auto& oscWaveChoice4 = *apvts.getRawParameterValue("OSC4WAVETYPE");

				float fmFreq = (float)* apvts.getRawParameterValue("FMFREQ1");
				float fmDepth = (float)* apvts.getRawParameterValue("FMDEPTH1");
				float fmFreq2 = (float)* apvts.getRawParameterValue("FMFREQ2");
				float fmDepth2 = (float)* apvts.getRawParameterValue("FMDEPTH2");
				float fmFreq3 = (float)* apvts.getRawParameterValue("FMFREQ3");
				float fmDepth3 = (float)* apvts.getRawParameterValue("FMDEPTH3");
				float fmFreq4 = (float)* apvts.getRawParameterValue("FMFREQ4");
				float fmDepth4 = (float)* apvts.getRawParameterValue("FMDEPTH4");

				float freqOSC1 = (float)* apvts.getRawParameterValue("OSC1FREQ");
				float freqOSC2 = (float)* apvts.getRawParameterValue("OSC2FREQ");
				float freqOSC3 = (float)* apvts.getRawParameterValue("OSC3FREQ");
				float freqOSC4 = (float)* apvts.getRawParameterValue("OSC4FREQ");

				//voice->getOscillator().setWaveType(oscWaveChoice);
				//voice->getOscillator().setOSCParams(oscStatus1, freqOSC1, fmFreq, fmDepth);
				//voice->getOscillator2().setWaveType(oscWaveChoice2);
				//voice->getOscillator2().setOSCParams(oscStatus2, freqOSC2, fmFreq2, fmDepth2);
				//voice->getOscillator3().setWaveType(oscWaveChoice3);
				//voice->getOscillator3().setOSCParams(oscStatus3, freqOSC3, fmFreq3, fmDepth3);
				//voice->getOscillator4().setWaveType(oscWaveChoice4);
				//voice->getOscillator4().setOSCParams(oscStatus4, freqOSC4, fmFreq4, fmDepth4);
				//voice->getOscillator5().setWaveType(oscWaveChoice5);
				//voice->getOscillator5().setOSCParams(oscStatus5, freqOSC5, fmFreq5, fmDepth5);
				//voice->getOscillator6().setWaveType(oscWaveChoice6);
				//voice->getOscillator6().setOSCParams(oscStatus6, freqOSC6, fmFreq6, fmDepth6);

				//int modOut1 = (int)* apvts.getRawParameterValue("OSCMODOUTPUT1");
				//int modOut2 = (int)* apvts.getRawParameterValue("OSCMODOUTPUT2");
				//int modOut3 = (int)* apvts.getRawParameterValue("OSCMODOUTPUT3");
				//int modOut4 = (int)* apvts.getRawParameterValue("OSCMODOUTPUT4");
				//int modOut5 = (int)* apvts.getRawParameterValue("OSCMODOUTPUT5");
				//int modOut6 = (int)* apvts.getRawParameterValue("OSCMODOUTPUT6");

				/*
				if (modOut1 > 0 && modOut1 < 7) {
					voice->getOscillator(1).setModWave(voice->getOscillator(modOut1).getWave());
				}
				if (modOut2 > 0 && modOut2 < 7) {
					voice->getOscillator(2).setModWave(voice->getOscillator(modOut2).getWave());
				}
				if (modOut3 > 0 && modOut3 < 7) {
					voice->getOscillator(3).setModWave(voice->getOscillator(modOut3).getWave());
				}
				if (modOut4 > 0 && modOut4 < 7) {
					voice->getOscillator(4).setModWave(voice->getOscillator(modOut4).getWave());
				}
				if (modOut5 > 0 && modOut5 < 7) {
					voice->getOscillator(5).setModWave(voice->getOscillator(modOut5).getWave());
				}
				if (modOut6 > 0 && modOut6 < 7) {
					voice->getOscillator(6).setModWave(voice->getOscillator(modOut6).getWave());
				}
				*/

				/*
				voice->getOscillator(1).setWaveType(oscWaveChoice);
				voice->getOscillator(1).setOSCParams(oscStatus1, freqOSC1, fmFreq, fmDepth);
				voice->getOscillator(2).setWaveType(oscWaveChoice2);
				voice->getOscillator(2).setOSCParams(oscStatus2, freqOSC2, fmFreq2, fmDepth2);
				voice->getOscillator(3).setWaveType(oscWaveChoice3);
				voice->getOscillator(3).setOSCParams(oscStatus3, freqOSC3, fmFreq3, fmDepth3);
				voice->getOscillator(4).setWaveType(oscWaveChoice4);
				voice->getOscillator(4).setOSCParams(oscStatus4, freqOSC4, fmFreq4, fmDepth4);
				voice->getOscillator(5).setWaveType(oscWaveChoice5);
				voice->getOscillator(5).setOSCParams(oscStatus5, freqOSC5, fmFreq5, fmDepth5);
				voice->getOscillator(6).setWaveType(oscWaveChoice6);
				voice->getOscillator(6).setOSCParams(oscStatus6, freqOSC6, fmFreq6, fmDepth6);
				*/

				voice->getOscillator().setWave(WTSin);
				//voice->getOscillator().setWaveType(oscWaveChoice);
				voice->getOscillator().setOSCParams(oscStatus1, freqOSC1, fmFreq, fmDepth);
				voice->getOscillator2().setWaveType(oscWaveChoice2);
				voice->getOscillator2().setOSCParams(oscStatus2, freqOSC2, fmFreq2, fmDepth2);
				voice->getOscillator3().setWaveType(oscWaveChoice3);
				voice->getOscillator3().setOSCParams(oscStatus3, freqOSC3, fmFreq3, fmDepth3);
				voice->getOscillator4().setWaveType(oscWaveChoice4);
				voice->getOscillator4().setOSCParams(oscStatus4, freqOSC4, fmFreq4, fmDepth4);

				auto& volOSC1 = *apvts.getRawParameterValue("VOLUME1");
				auto& volOSC2 = *apvts.getRawParameterValue("VOLUME2");
				auto& volOSC3 = *apvts.getRawParameterValue("VOLUME3");
				auto& volOSC4 = *apvts.getRawParameterValue("VOLUME4");
				auto& vol = *apvts.getRawParameterValue("VOLUME");

				voice->update(attack.load(), decay.load(), sustain.load(), releaze.load(), volOSC1.load(), volOSC2.load(), volOSC3.load(), volOSC4.load(), vol.load());

				//voice->updaterFilter(

				//LFO
			}
	}

	for (const MidiMessageMetadata metadata : midiMessages)
		if (metadata.numBytes == 3)
			Logger::writeToLog("TimeStamp: " + juce::String (metadata.getMessage().getTimeStamp()));

	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

	
	//Filter stuff

	//***
	int FilterType = (int) *apvts.getRawParameterValue("FILTERTYPE"); //не сменяются типы
	float Resonanse = (float) *apvts.getRawParameterValue("FILTERRES");
	float CutOff = (float) *apvts.getRawParameterValue("FILTERCUTOFF");
	//*** order: 1, 1.f, 100

	filter.updateParams(FilterType, Resonanse, CutOff);

	filter.process(buffer);

}

//==============================================================================
bool SynthAlphaAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynthAlphaAudioProcessor::createEditor()
{
    return new SynthAlphaAudioProcessorEditor (*this);
}

//==============================================================================
void SynthAlphaAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynthAlphaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout SynthAlphaAudioProcessor::createParams()
{
	//ComboBox Type Wave
	//Attack - float
	//Decay - float
	//Sustain - float
	//Relize - float

	std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

	//////////////////////Volume 
	params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME", "Volume", juce::NormalisableRange<float> {0.0f, 100.0f, 0.01f}, 60.0f));

	//OSC select
	//params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC", "Oscillator", 	juce::StringArray {"Sine", "Saw", "Square"}, 0));
	//params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2", "Oscillator 2", juce::StringArray{ "Sine", "Saw", "Square" }, 0));

	//params.push_back(std::make_unique<juce::AudioParameterChoice>("MODWAVETYPE", "Modulation Wave Type", juce::StringArray{ "Sine", "Saw", "Square" }, 0));
	
	/*
	juce::StringArray choisesOSC;
	choisesOSC.add("Free");
	for (int i = 1; i <= osc.OSCCount; i++) {
		choisesOSC.add("OSC" + String(i));
	}
	*/

    //////////////////////OSC1
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1STATUS", "Osc 1 Status", juce::StringArray{ "G&M", "MOD" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODINPUT1", "Osc 1 Modulation Input", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODOUTPUT1", "Osc 1 Modulation Output", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc 1 Wave Type", juce::StringArray{ "Sine", "Saw", "Square" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMFREQ1", "Osc 1 FM Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMDEPTH1", "Osc 1 FM Depth", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1FREQ", "Osc 1 Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 5.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME1", "Osc 1 Volume", juce::NormalisableRange<float> {0.0f, 100.0f, 0.01f}, 60.0f));
	//////////////////////OSC2
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2STATUS", "Osc 2 Status", juce::StringArray{ "G&M", "MOD" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODINPUT2", "Osc 2 Modulation Input", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODOUTPUT2", "Osc 2 Modulation Output", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2WAVETYPE", "Osc 2 Wave Type", juce::StringArray{ "Sine", "Saw", "Square" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMFREQ2", "Osc 2 FM Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMDEPTH2", "Osc 2 FM Depth", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2FREQ", "Osc 2 Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 5.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME2", "Osc 2 Volume", juce::NormalisableRange<float> {0.0f, 100.0f, 0.01f}, 60.0f));
	//////////////////////OSC3
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC3STATUS", "Osc 3 Status", juce::StringArray{ "G&M", "MOD" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODINPUT3", "Osc 3 Modulation Input", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODOUTPUT3", "Osc 3 Modulation Output", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC3WAVETYPE", "Osc 3 Wave Type", juce::StringArray{ "Sine", "Saw", "Square" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMFREQ3", "Osc 3 FM Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMDEPTH3", "Osc 3 FM Depth", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC3FREQ", "Osc 3 Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 5.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME3", "Osc 3 Volume", juce::NormalisableRange<float> {0.0f, 100.0f, 0.01f}, 60.0f));
	//////////////////////OSC4
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC4STATUS", "Osc 4 Status", juce::StringArray{ "G&M", "MOD" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODINPUT4", "Osc 4 Modulation Input", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterInt>("OSCMODOUTPUT4", "Osc 4 Modulation Output", 1, 10, 0));////////////
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC4WAVETYPE", "Osc 4 Wave Type", juce::StringArray{ "Sine", "Saw", "Square" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMFREQ4", "Osc 4 FM Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FMDEPTH4", "Osc 4 FM Depth", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC4FREQ", "Osc 4 Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 5.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME4", "Osc 4 Volume", juce::NormalisableRange<float> {0.0f, 100.0f, 0.01f}, 60.0f));

	//////////////////////ADSR
	params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float>{0.00001f, 1.0f, 0.001f}, 0.1f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float>{0.1f, 1.0f, 0.001f}, 0.1f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>{0.1f, 1.0f, 0.001f}, 1.0f));
	                                                                                                                                     //start value !!
	params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEAZE", "Releaze", juce::NormalisableRange<float>{0.1f, 4.0f, 0.001f}, 0.4f));

	//////////////////////filter
	params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTERTYPE", "Filter Type", juce::StringArray{"LowPass", "BandPass", "HighPass"}, 0));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERCUTOFF", "Filter CutOff", juce::NormalisableRange<float>{20.0f, 20000.0f, 0.1f, 0.6f}, 200.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERRES", "Filter Resonance", juce::NormalisableRange<float>{1.0f, 10.0f, 0.1f}, 1.0f));
	//////////////////////filter ADSR 
	params.push_back(std::make_unique<juce::AudioParameterFloat>("MODATTACK", "Mod Attack", juce::NormalisableRange<float>{0.00001f, 1.0f, 0.001f}, 0.1f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("MODDECAY", "Mod Decay", juce::NormalisableRange<float>{0.1f, 1.0f, 0.001f}, 0.1f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("MODSUSTAIN", "Mod Sustain", juce::NormalisableRange<float>{0.1f, 1.0f, 0.001f}, 1.0f));

	return { params.begin(), params.end() };
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthAlphaAudioProcessor();
}
