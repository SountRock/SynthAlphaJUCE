/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAlphaAudioProcessorEditor::SynthAlphaAudioProcessorEditor (SynthAlphaAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), 
	osc(audioProcessor.apvts, "OSC1STATUS", "OSC1WAVETYPE", "OSCMODINPUT1", "OSCMODOUTPUT1", "FMFREQ1", "FMDEPTH1", "OSC1FREQ", "VOLUME1", 1),
	osc2(audioProcessor.apvts, "OSC2STATUS", "OSC2WAVETYPE", "OSCMODINPUT2", "OSCMODOUTPUT2", "FMFREQ2", "FMDEPTH2", "OSC2FREQ", "VOLUME2", 2),
	osc3(audioProcessor.apvts, "OSC3STATUS", "OSC3WAVETYPE", "OSCMODINPUT3", "OSCMODOUTPUT3", "FMFREQ3", "FMDEPTH3", "OSC3FREQ", "VOLUME3", 3),
	osc4(audioProcessor.apvts, "OSC4STATUS", "OSC4WAVETYPE", "OSCMODINPUT4", "OSCMODOUTPUT4", "FMFREQ4", "FMDEPTH4", "OSC4FREQ", "VOLUME4", 4),
	adsr(audioProcessor.apvts),
	filter(audioProcessor.apvts, "FILTERTYPE", "FILTERCUTOFF", "FILTERRES", "VOLUME")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 600);
	setResizable(true, true);
	background1 = ImageCache::getFromMemory(BinaryData::mramor1_jpg, BinaryData::mramor1_jpgSize);

	juce::StringArray choises{ "Sine", "Saw", "Square" };
	juce::StringArray choisesOSC;
	choisesOSC.add("None");
	for (int i = 1; i <= osc.OSCCount; i++) {
		choisesOSC.add("OSC" + String(i));
	}

	//////////////////////OSC1
	osc.setComboBoxChoise(osc.oscWaveSel, choises);
	osc.setComboBoxChoise(osc.modOscSel, choisesOSC);
	//////////////////////OSC2
	osc2.setComboBoxChoise(osc2.oscWaveSel, choises);
	osc2.setComboBoxChoise(osc2.modOscSel, choisesOSC);
	//////////////////////OSC3
	osc3.setComboBoxChoise(osc3.oscWaveSel, choises);
	osc3.setComboBoxChoise(osc3.modOscSel, choisesOSC);
	//////////////////////OSC4
	osc4.setComboBoxChoise(osc4.oscWaveSel, choises);
    osc4.setComboBoxChoise(osc4.modOscSel, choisesOSC);

	addAndMakeVisible(osc);
	addAndMakeVisible(osc2);
	addAndMakeVisible(osc3);
	addAndMakeVisible(osc4);
	addAndMakeVisible(adsr);
	addAndMakeVisible(filter);
	
}

SynthAlphaAudioProcessorEditor::~SynthAlphaAudioProcessorEditor()
{
}

//==============================================================================
void SynthAlphaAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
	g.drawImage(background1, getLocalBounds().toFloat());
}

void SynthAlphaAudioProcessorEditor::resized()
{
	osc.setBounds(10, 10, getWidth() * 0.15f - 2, getHeight() * 0.6f);
	osc2.setBounds(osc.getX() + osc.getWidth(), 10, getWidth() * 0.15f - 2, getHeight() * 0.6f);
	osc3.setBounds(osc2.getX() + osc2.getWidth(), 10, getWidth() * 0.15f - 2, getHeight() * 0.6f);
	osc4.setBounds(osc3.getX() + osc3.getWidth(), 10, getWidth() * 0.15f - 2, getHeight() * 0.6f);
	adsr.setBounds(getWidth() * 0.6 + 10, 10, getWidth() * 0.4f, getHeight() * 0.55f);
	filter.setBounds(20, osc.getY() + osc.getHeight() - 45, getWidth() / 2, getHeight() * 0.45f);
	//Logger::writeToLog(String(osc2.getNumberOsc()));
	//Logger::writeToLog(String(osc.OSCCount));
}



