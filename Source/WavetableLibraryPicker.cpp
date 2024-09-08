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
WavetableLibraryPicker::WavetableLibraryPicker(DistortionEngine& distortionEngine, juce::ValueTree libraryPathParam)
	: distortionEngine(distortionEngine), libraryPathParam(libraryPathParam), symmetricMode(true), forceAscending(false)
{
	addAndMakeVisible(wavetableLibraryLoadButton);
	addAndMakeVisible(wavetableLibraryFolderLabel);
	addAndMakeVisible(currentWavetableFileLabel);
	addAndMakeVisible(prevWavetableButton);
	addAndMakeVisible(nextWavetableButton);

	audioFormatManager.registerBasicFormats();

	currentWavetableFileIndex = 0;

	wavetableLibraryLoadButton.setButtonText("Choose wavetable library folder...");
	wavetableLibraryLoadButton.onClick = [this]()
		{
			chooseWavetableLibraryFolder();
		};

	prevWavetableButton.setButtonText("<");
	prevWavetableButton.onClick = [this]()
		{
			prevWavetable();
		};
	nextWavetableButton.setButtonText(">");
	nextWavetableButton.onClick = [this]()
		{
			nextWavetable();
		};

	updateWavetableLibraryFolder(libraryPathParam.getProperty("path").toString());
	loadWavetableFromFile(libraryPathParam.getProperty("fileName").toString());
}

WavetableLibraryPicker::~WavetableLibraryPicker()
{
	delete[] wavetableBuffer;
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
	prevWavetableButton.setBounds(bounds.removeFromLeft(smallButtonWidth));
	nextWavetableButton.setBounds(bounds.removeFromRight(smallButtonWidth));
	currentWavetableFileLabel.setBounds(bounds);
}

void WavetableLibraryPicker::chooseWavetableLibraryFolder()
{
	wavetableLibraryFolderChooser = std::make_unique<juce::FileChooser>("Select wavetable library dir...", juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory));

	auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
	wavetableLibraryFolderChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
		{
			updateWavetableLibraryFolder(chooser.getResult());
			resetActiveWavetableFile();
		});
}

void WavetableLibraryPicker::loadWavetableFromFile()
{
	auto* reader = audioFormatManager.createReaderFor(wavetableFiles[currentWavetableFileIndex]);
	if (reader != nullptr)
	{
		if (wavetableBuffer == nullptr)
			wavetableBuffer = new float[MORPHSHAPER_WAVETABLE_RESOLUTION];

		int numFullFrames = juce::jmin(static_cast<int>(reader->lengthInSamples / MORPHSHAPER_WAVETABLE_RESOLUTION / 4), 32);
		std::vector<WavetableFunction> funcs;
		for (int i = 0; i < numFullFrames; i++)
		{
			// TODO - delete this! this is just temporary shenanigans to ensure compatibility
			// with serum wavetables for initial testing, which happen to be 2048 samples per frame
			float* tempBuffer = new float[MORPHSHAPER_WAVETABLE_RESOLUTION * 4];
			reader->read(&tempBuffer, 1, MORPHSHAPER_WAVETABLE_RESOLUTION * i * 4, MORPHSHAPER_WAVETABLE_RESOLUTION * 4);
			for (int j = 0; j < MORPHSHAPER_WAVETABLE_RESOLUTION; j++)
			{
				wavetableBuffer[j] = tempBuffer[j * 4];
			}
			delete[] tempBuffer;

			if (symmetricMode)
			{
				transformDataToSymmetric();
			}

			funcs.push_back(WavetableFunction(wavetableBuffer));
		}
		// The caller is responsible to free the reader
		delete reader;
		// TODO handle errors...

		distortionEngine.setWavetable(funcs);

		repaint();
	}
	// TODO tell the user the file makes no sense otherwise!
}

void WavetableLibraryPicker::transformDataToSymmetric()
{
	// fill the upper half of the buffer with "squished" signal
	for (int i = MORPHSHAPER_WAVETABLE_RESOLUTION - 1, j = i - 1; i >= MORPHSHAPER_WAVETABLE_RESOLUTION / 2; i--, j-=2)
	{
		wavetableBuffer[i] = wavetableBuffer[j];
		if(forceAscending) wavetableBuffer[i] = wavetableBuffer[i] * 0.5f + 0.5f;
	}

	// symmetrically copy over right side of the buffer
	for (int i = 0; i < MORPHSHAPER_WAVETABLE_RESOLUTION / 2; i++)
	{
		wavetableBuffer[i] = -wavetableBuffer[i + MORPHSHAPER_WAVETABLE_RESOLUTION / 2];
	}

	// zero out the middle part
	wavetableBuffer[MORPHSHAPER_WAVETABLE_RESOLUTION / 2] = 0;
}

void WavetableLibraryPicker::loadWavetableFromFile(const juce::String& wtFile)
{
	for (int i = 0; i < wavetableFiles.size(); ++i)
	{
		if (!wavetableFiles[i].getFileName().compare(wtFile))
		{
			currentWavetableFileIndex = i;
			currentWavetableFileLabel.setText(wavetableFiles[currentWavetableFileIndex].getFileName(), juce::dontSendNotification);
			loadWavetableFromFile();
			return;
		}
	}
}

void WavetableLibraryPicker::updateWavetableLibraryFolder(juce::File newWtFolder)
{
	wavetableLibraryFolder = newWtFolder;
	wavetableFiles = wavetableLibraryFolder.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false);
	wavetableLibraryFolderLabel.setText(wavetableLibraryFolder.getFileName(), juce::dontSendNotification);
}

void WavetableLibraryPicker::resetActiveWavetableFile()
{
	if (!wavetableFiles.isEmpty())
	{
		libraryPathParam.setProperty("path", wavetableLibraryFolder.getFullPathName(), nullptr);
		currentWavetableFileIndex = 0;
		libraryPathParam.setProperty("fileName", wavetableFiles[currentWavetableFileIndex].getFileName(), nullptr);
		currentWavetableFileLabel.setText(wavetableFiles[currentWavetableFileIndex].getFileName(), juce::dontSendNotification);
		loadWavetableFromFile();
	}
}

void WavetableLibraryPicker::prevWavetable()
{
	if (!wavetableFiles.isEmpty())
	{
		currentWavetableFileIndex = (currentWavetableFileIndex - 1);
		if (currentWavetableFileIndex < 0) currentWavetableFileIndex = wavetableFiles.size() - 1;
		loadWavetableFromFile();
		libraryPathParam.setProperty("fileName", wavetableFiles[currentWavetableFileIndex].getFileName(), nullptr);
		currentWavetableFileLabel.setText(wavetableFiles[currentWavetableFileIndex].getFileName(), juce::dontSendNotification);
	}
}

void WavetableLibraryPicker::nextWavetable()
{
	if (!wavetableFiles.isEmpty())
	{
		currentWavetableFileIndex = (currentWavetableFileIndex + 1) % wavetableFiles.size();
		loadWavetableFromFile();
		libraryPathParam.setProperty("fileName", wavetableFiles[currentWavetableFileIndex].getFileName(), nullptr);
		currentWavetableFileLabel.setText(wavetableFiles[currentWavetableFileIndex].getFileName(), juce::dontSendNotification);
	}
}

