/*
  ==============================================================================

    WavetableLibraryPicker.h
    Created: 2 Jul 2024 10:40:22pm
    Author:  Nemanja

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WavetableLibraryPicker  : public juce::Component
{
public:
    WavetableLibraryPicker();
    ~WavetableLibraryPicker() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void chooseWavetableLibraryFolder();

    juce::TextButton wavetableLibraryLoadButton;
    juce::File wavetableLibraryFolder;
    juce::Label wavetableLibraryFolderLabel;
    std::unique_ptr<juce::FileChooser> wavetableLibraryFolderChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableLibraryPicker)
};
