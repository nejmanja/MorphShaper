/*
  ==============================================================================

	LFOEditor.h
	Created: 21 Sep 2024 4:21:58pm
	Author:  Nemanja

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ModulationMatrix.h"

//==============================================================================
/*
*/
class LFOEditor : public juce::Component
{
public:
	LFOEditor(juce::AudioProcessorValueTreeState& vts, ModulationMatrix& modulationMatrix, ModulationMatrix::ModulationSource modSource);
	~LFOEditor() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	void changeModulationDestination();

	typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
	typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

	juce::ComboBox lfoTypeComboBox;
	juce::ComboBox lfoTargetComboBox;
	juce::Slider frequencySlider, modulationIntensitySlider;
	std::unique_ptr<SliderAttachment> frequencySliderAttachment, modulationIntensitySliderAttachment;
	std::unique_ptr<ComboBoxAttachment> targetComboBoxAttachment;
	
	juce::Label frequencySliderLabel, modulationIntensitySliderLabel;

	ModulationMatrix& modulationMatrix;
	// which LFO is this editor for?
	ModulationMatrix::ModulationSource modSource;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOEditor)
};
