/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "FilterPluginParameters.h"

//==============================================================================
MorphShaperAudioProcessor::MorphShaperAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
	,
	parameters(*this, nullptr, juce::Identifier("MorphShaper"),
		{
			std::make_unique<juce::AudioParameterFloat>("wtPosition", "Wavetable Position", 0.0f, 1.0f, 0.0f),
			std::make_unique<juce::AudioParameterFloat>("preGain", "Pre Gain", -100.0f, 30.0f, 0.0f),
			std::make_unique<juce::AudioParameterFloat>("postGain", "Post Gain", -100.0f, 30.0f, 0.0f),
			std::make_unique<juce::AudioParameterBool>("preFilterEnabled", "Pre Filter Enabled", false),
			std::make_unique<juce::AudioParameterChoice>("preFilterType", "Pre Filter Type", juce::StringArray{"Lowpass", "HighPass", "BandPass"}, 0),
			std::make_unique<juce::AudioParameterFloat>("preFilterCutoff", "Pre Filter Cutoff Frequency", juce::NormalisableRange<float>(20, 20000, 0, 0.7f), 1000.0f),
			std::make_unique<juce::AudioParameterFloat>("preFilterResonance", "Pre Filter Resonance", juce::NormalisableRange<float>(0.5f, 10.0f, 0.0f, 0.7f), 0.7f),
			std::make_unique<juce::AudioParameterBool>("postFilterEnabled", "Post Filter Enabled", false),
			std::make_unique<juce::AudioParameterChoice>("postFilterType", "Post Filter Type", juce::StringArray{"Lowpass", "HighPass", "BandPass"}, 0),
			std::make_unique<juce::AudioParameterFloat>("postFilterCutoff", "Post Filter Cutoff Frequency", juce::NormalisableRange<float>(20, 20000, 0, 0.7f), 1000.0f),
			std::make_unique<juce::AudioParameterFloat>("postFilterResonance", "Post Filter Resonance", juce::NormalisableRange<float>(0.5f, 10.0f, 0.0f, 0.7f), 0.7f),
		})
{

	juce::ValueTree libraryPathParam(juce::Identifier("wtLibraryPath"));
	libraryPathParam.setProperty("path", "", nullptr);
	libraryPathParam.setProperty("fileName", "", nullptr);
	parameters.state.addChild(libraryPathParam, 0, nullptr);
	wavetablePositionParameter = parameters.getRawParameterValue("wtPosition");
	preGainParameter = parameters.getRawParameterValue("preGain");
	postGainParameter = parameters.getRawParameterValue("postGain");

	distortionEngine.reset(new DistortionEngine(
		wavetablePositionParameter,
		preGainParameter,
		postGainParameter,
		FilterPluginParameters
		{
			parameters.getRawParameterValue("preFilterCutoff"),
			parameters.getRawParameterValue("preFilterResonance"),
			parameters.getRawParameterValue("preFilterEnabled"),
			parameters.getRawParameterValue("preFilterType")
		},
		FilterPluginParameters
		{
			parameters.getRawParameterValue("postFilterCutoff"),
			parameters.getRawParameterValue("postFilterResonance"),
			parameters.getRawParameterValue("postFilterEnabled"),
			parameters.getRawParameterValue("postFilterType")
		}
	));

	lfo.initialise([](float x) { return std::sin(x); }, 128);
	lfo.setFrequency(3.0f);
}

MorphShaperAudioProcessor::~MorphShaperAudioProcessor()
{
}

//==============================================================================
const juce::String MorphShaperAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool MorphShaperAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool MorphShaperAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool MorphShaperAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double MorphShaperAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int MorphShaperAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int MorphShaperAudioProcessor::getCurrentProgram()
{
	return 0;
}

void MorphShaperAudioProcessor::setCurrentProgram(int index)
{
	juce::ignoreUnused(index);
}

const juce::String MorphShaperAudioProcessor::getProgramName(int index)
{
	juce::ignoreUnused(index);
	return {};
}

void MorphShaperAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
	juce::ignoreUnused(index);
	juce::ignoreUnused(newName);
}

//==============================================================================
void MorphShaperAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
	distortionEngine->prepare({ sampleRate, (juce::uint32)samplesPerBlock, (juce::uint32)getTotalNumOutputChannels() });
	
	lfo.prepare({ sampleRate / lfoUpdateRate, (juce::uint32)samplesPerBlock, (juce::uint32)getTotalNumOutputChannels() });
}

void MorphShaperAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MorphShaperAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void MorphShaperAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ignoreUnused(midiMessages);

	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	auto block = juce::dsp::AudioBlock<float>(buffer);
	auto context = juce::dsp::ProcessContextReplacing<float>(block);

	auto max = juce::jmin((size_t)buffer.getNumSamples(), lfoUpdateCounter);
	lfoUpdateCounter -= max;

	if (lfoUpdateCounter == 0)
	{
		lfoUpdateCounter = lfoUpdateRate;
		auto lfoOut = lfo.processSample(0.0f);
		auto modulatorValue = juce::jmap(lfoOut, -1.0f, 1.0f, -0.5f, 0.5f);
		distortionEngine->setLfoModulatorValue(modulatorValue);
	}

	distortionEngine->process(context);
}

//==============================================================================
bool MorphShaperAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MorphShaperAudioProcessor::createEditor()
{
	return new MorphShaperAudioProcessorEditor(*this, parameters);
}

//==============================================================================
void MorphShaperAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
	auto state = parameters.copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void MorphShaperAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
	std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
	{
		if (xmlState->hasTagName(parameters.state.getType()))
		{
			parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
		}
	}
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new MorphShaperAudioProcessor();
}
