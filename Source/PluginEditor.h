/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UInterface//ADSRComponent.h"
#include "UInterface//OSCComponent.h"
#include "UInterface//FilterComponent.h"

//==============================================================================
/**
*/
class SynthAlphaAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    SynthAlphaAudioProcessorEditor(SynthAlphaAudioProcessor&);
    ~SynthAlphaAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

	//int getAmountOsc();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthAlphaAudioProcessor& audioProcessor;
	Image background1;

	OSCComponent osc;
	OSCComponent osc2;
	OSCComponent osc3;
	OSCComponent osc4;

	ADSRComponent adsr;
	FilterComponent filter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthAlphaAudioProcessorEditor)
};
