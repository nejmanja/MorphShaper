/*
  ==============================================================================

    WavetableLibraryPicker.h
    Created: 2 Jul 2024 10:40:22pm
    Author:  Nemanja

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DistortionEngine.h"
#include "WavetableDrawer.h"

//==============================================================================
/*
*/
class WavetableLibraryPicker  : public juce::Component
{
public:
    WavetableLibraryPicker(DistortionEngine& distortionEngine, WavetableDrawer& drawer, juce::ValueTree libraryPathParam);
    ~WavetableLibraryPicker() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void chooseWavetableLibraryFolder();

    void prevWavetable();
    void nextWavetable();

    void loadWavetableFromFile();
    void transformDataToSymmetric();
    void loadWavetableFromFile(const juce::String& wtFile);
    void updateWavetableLibraryFolder(juce::File newWtFolder);
    void resetActiveWavetableFile();

    // The library picker refreshes the drawer upon loading a WT from a file
    WavetableDrawer& drawer;

    juce::TextButton wavetableLibraryLoadButton;
    juce::File wavetableLibraryFolder;
    juce::Array<juce::File> wavetableFiles;
    int currentWavetableFileIndex;

    juce::ToggleButton symmetricModeButton, ascendingModeButton;
    juce::Label symmetricModeLabel, ascendingModeLabel;
    bool symmetricMode, ascendingMode;

    float* wavetableBuffer = nullptr;

    juce::Label wavetableLibraryFolderLabel, currentWavetableFileLabel;
    juce::TextButton prevWavetableButton, nextWavetableButton;
    std::unique_ptr<juce::FileChooser> wavetableLibraryFolderChooser;
    juce::AudioFormatManager audioFormatManager;
    juce::ValueTree libraryPathParam;

    DistortionEngine& distortionEngine;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableLibraryPicker)
};
