/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "DistortionEditor.h"
#include "WavetableLibraryPicker.h"

//==============================================================================
/**
*/
class MorphShaperAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MorphShaperAudioProcessorEditor (MorphShaperAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~MorphShaperAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MorphShaperAudioProcessor& audioProcessor;
    DistortionEditor distortionEditor;
    WavetableLibraryPicker wavetableLibraryPicker;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MorphShaperAudioProcessorEditor)
};
