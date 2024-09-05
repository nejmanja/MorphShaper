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
#include "WavetableLibraryPicker.h"

//==============================================================================
/*
*/

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class DistortionEditor  : public juce::Component, public juce::AudioProcessorValueTreeState::Listener
{
public:
    DistortionEditor(DistortionEngine& distortionEngine, juce::AudioProcessorValueTreeState& vts);
    ~DistortionEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void parameterChanged(const juce::String& parameterID, float newValue) override;
    
private:
    juce::Point<float> getPointInWavetable(int sampleIdx, juce::Rectangle<int> bounds, float widthScale, float heightScale);
    void generateWavetablePath(juce::Rectangle<int> bounds);

    juce::Path wavetablePath;
    DistortionEngine& distortionEngine;
    WavetableLibraryPicker wavetableLibraryPicker;

    float currentWaveform[MORPHSHAPER_WAVETABLE_RESOLUTION] {0};
    juce::Slider preGainSlider, postGainSlider, modulationSlider;
    std::unique_ptr<SliderAttachment> modulationSliderAttachment;
    juce::Label preGainSliderLabel, postGainSliderLabel, modulationSliderLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionEditor)

};
