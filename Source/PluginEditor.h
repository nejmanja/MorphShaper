/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "DistortionEditor.h"
#include "WavetableDrawer.h"
#include "FilterEditor.h"
#include "LFOEditor.h"
#include "ModulationMatrix.h"

//==============================================================================
/**
*/
class MorphShaperAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	MorphShaperAudioProcessorEditor(MorphShaperAudioProcessor&, juce::AudioProcessorValueTreeState&, ModulationMatrix&);
	~MorphShaperAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

private:
	void modulationParameterChanged(float newValue);
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	MorphShaperAudioProcessor& audioProcessor;
	
	DistortionEditor distortionEditor;
	WavetableDrawer wavetableDrawer;
	WavetableLibraryPicker wavetableLibraryPicker;
	FilterEditor preFilterEditor, postFilterEditor;
	LFOEditor lfoEditor;

	juce::AudioProcessorValueTreeState& valueTreeState;
	std::unique_ptr<juce::ParameterAttachment> modulationParamAttachment;
	juce::Label titleLabel;

	std::function<void(float)> modulationParamChangedCallback;

	std::array<float, MORPHSHAPER_WAVETABLE_RESOLUTION> currentWavetable;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MorphShaperAudioProcessorEditor)
};
