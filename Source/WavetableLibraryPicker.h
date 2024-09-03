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

    void prevWavetable();
    void nextWavetable();

    void loadWavetableFromFile();

    juce::TextButton wavetableLibraryLoadButton;
    juce::File wavetableLibraryFolder;
    juce::Array<juce::File> wavetableFiles;
    int currentWavetableFileIndex;

    float* wavetableBuffer = nullptr;

    juce::Label wavetableLibraryFolderLabel, currentWavetableFileLabel;
    juce::TextButton prevWavetableButton, nextWavetableButton;
    std::unique_ptr<juce::FileChooser> wavetableLibraryFolderChooser;
    juce::AudioFormatManager audioFormatManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableLibraryPicker)
};
