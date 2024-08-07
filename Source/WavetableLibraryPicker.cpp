/*
  ==============================================================================

    WavetableLibraryPicker.cpp
    Created: 2 Jul 2024 10:40:22pm
    Author:  Nemanja

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WavetableLibraryPicker.h"

//==============================================================================
WavetableLibraryPicker::WavetableLibraryPicker()
{
    addAndMakeVisible(wavetableLibraryLoadButton);
    addAndMakeVisible(wavetableLibraryFolderLabel);

    wavetableLibraryLoadButton.setButtonText("Choose wavetable library folder...");
    wavetableLibraryLoadButton.onClick = [this]()
    {
        chooseWavetableLibraryFolder();
    };
}

WavetableLibraryPicker::~WavetableLibraryPicker()
{
}

void WavetableLibraryPicker::paint (juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void WavetableLibraryPicker::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto thirdHeight = getHeight() / 3;
    wavetableLibraryLoadButton.setBounds(0, 0, getWidth(), thirdHeight * 2);
    wavetableLibraryFolderLabel.setBounds(0, thirdHeight * 2, getWidth(), thirdHeight);

}

void WavetableLibraryPicker::chooseWavetableLibraryFolder()
{
    wavetableLibraryFolderChooser = std::make_unique<juce::FileChooser>("Select wavetable library dir...", juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory));

    auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
    wavetableLibraryFolderChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
        {
            wavetableLibraryFolder = chooser.getResult();
            wavetableLibraryFolderLabel.setText(wavetableLibraryFolder.getFullPathName(), juce::dontSendNotification);
        });
}