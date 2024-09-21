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
class LFOEditor  : public juce::Component
{
public:
    LFOEditor(juce::AudioProcessorValueTreeState& vts, ModulationMatrix& modulationMatrix);
    ~LFOEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void changeModulationDestination();

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    juce::ComboBox lfoTypeComboBox;
    juce::ComboBox lfoTargetComboBox;
    juce::Slider frequencySlider;
    juce::Slider modulationIntensitySlider;
    std::unique_ptr<SliderAttachment> frequencySliderAttachment;
    std::unique_ptr<SliderAttachment> modulationIntensitySliderAttachment;

    ModulationMatrix& modulationMatrix;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOEditor)
};
