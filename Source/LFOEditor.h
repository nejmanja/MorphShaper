/*
  ==============================================================================

    LFOEditor.h
    Created: 21 Sep 2024 4:21:58pm
    Author:  Nemanja

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LFOEditor  : public juce::Component
{
public:
    LFOEditor(std::atomic<float>* frequencyParamter);
    ~LFOEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox lfoTypeComboBox;
    juce::Slider frequencySlider;

    std::atomic<float>* frequencyParameter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOEditor)
};
