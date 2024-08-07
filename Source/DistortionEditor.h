/*
  ==============================================================================

    DistortionEditor.h
    Created: 23 Mar 2024 9:36:24pm
    Author:  Nemanja

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DistortionEngine.h"

//==============================================================================
/*
*/
class DistortionEditor  : public juce::Component
{
public:
    DistortionEditor(DistortionEngine& distortionEngine);
    ~DistortionEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    DistortionEngine& distortionEngine;
    float currentWaveform[MORPHSHAPER_WAVETABLE_RESOLUTION] {0};
    juce::Slider preGainSlider, postGainSlider, modulationSlider;
    juce::Label preGainSliderLabel, postGainSliderLabel, modulationSliderLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionEditor)
};
