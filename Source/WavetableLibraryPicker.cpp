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
	addAndMakeVisible(currentWavetableFileLabel);
	addAndMakeVisible(previousWavetableButton);
	addAndMakeVisible(nextWavetableButton);

	audioFormatManager.registerBasicFormats();

	currentWavetableFileIndex = 0;

	wavetableLibraryLoadButton.setButtonText("Choose wavetable library folder...");
	wavetableLibraryLoadButton.onClick = [this]()
		{
			chooseWavetableLibraryFolder();
		};

	previousWavetableButton.setButtonText("<");
	previousWavetableButton.onClick = [this]()
		{
			if (!wavetableFiles.isEmpty())
			{
				currentWavetableFileIndex = (currentWavetableFileIndex - 1);
				if (currentWavetableFileIndex < 0) currentWavetableFileIndex = wavetableFiles.size() - 1;
				currentWavetableFileLabel.setText(wavetableFiles[currentWavetableFileIndex].getFileName(), juce::dontSendNotification);
			}
		};
	nextWavetableButton.setButtonText(">");
	nextWavetableButton.onClick = [this]()
		{
			if (!wavetableFiles.isEmpty())
			{
				currentWavetableFileIndex = (currentWavetableFileIndex + 1) % wavetableFiles.size();
				currentWavetableFileLabel.setText(wavetableFiles[currentWavetableFileIndex].getFileName(), juce::dontSendNotification);
			}
		};
}

WavetableLibraryPicker::~WavetableLibraryPicker()
{
}

void WavetableLibraryPicker::paint(juce::Graphics& g)
{
	juce::ignoreUnused(g);
}

void WavetableLibraryPicker::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	auto bounds = getLocalBounds();
	auto thirdHeight = bounds.getHeight() / 3;
	auto smallButtonWidth = bounds.getWidth() / 8;
	wavetableLibraryLoadButton.setBounds(bounds.removeFromTop(thirdHeight));
	wavetableLibraryFolderLabel.setBounds(bounds.removeFromTop(thirdHeight));
	previousWavetableButton.setBounds(bounds.removeFromLeft(smallButtonWidth));
	nextWavetableButton.setBounds(bounds.removeFromRight(smallButtonWidth));
	currentWavetableFileLabel.setBounds(bounds);
}

void WavetableLibraryPicker::chooseWavetableLibraryFolder()
{
	wavetableLibraryFolderChooser = std::make_unique<juce::FileChooser>("Select wavetable library dir...", juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory));

	auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
	wavetableLibraryFolderChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
		{
			wavetableLibraryFolder = chooser.getResult();
			wavetableFiles = wavetableLibraryFolder.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false);
			wavetableLibraryFolderLabel.setText(wavetableLibraryFolder.getFullPathName(), juce::dontSendNotification);

			if (!wavetableFiles.isEmpty()) {
				currentWavetableFileLabel.setText(wavetableFiles[currentWavetableFileIndex].getFileName(), juce::dontSendNotification);
			}
		});
}