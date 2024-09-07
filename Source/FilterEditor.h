/*
  ==============================================================================

    FilterEditor.h
    Created: 7 Sep 2024 7:44:59pm
    Author:  Nemanja

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FilterEditor  : public juce::Component
{
public:
    FilterEditor(const juce::String& title, juce::AudioProcessorValueTreeState& vts, const juce::String& frequencyParamID, const juce::String& resonanceParamIDconst, const juce::String& enabledParamID, const juce::String& typeParamID);
    ~FilterEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

    juce::ComboBox filterTypeComboBox;
    std::unique_ptr<ComboBoxAttachment> filterTypeAttachment;
    juce::Slider frequencySlider, resonanceSlider;
    std::unique_ptr<SliderAttachment> frequencySliderAttachment, resonanceSliderAttachment;
    juce::Label titleLabel, frequencyLabel, resonanceLabel;
    juce::ToggleButton enabledToggle;

    juce::AudioProcessorValueTreeState& vts;
    const juce::String enabledParamID;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterEditor)
};
