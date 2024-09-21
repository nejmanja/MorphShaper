/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DistortionEngine.h"
#include "ModulationMatrix.h"

//==============================================================================
/**
*/
class MorphShaperAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MorphShaperAudioProcessor();
    ~MorphShaperAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    DistortionEngine& getDistortionEngine() { return *distortionEngine; }

private:
    juce::AudioProcessorValueTreeState createPluginParameters();

    std::unique_ptr<ModulationMatrix> modulationMatrix;

    static constexpr size_t lfoUpdateRate = 100;
    size_t lfoUpdateCounter = lfoUpdateRate;
    juce::dsp::Oscillator<float> lfo;

    juce::AudioProcessorValueTreeState parameters;
    std::atomic<float>* wavetablePositionParameter;
    std::atomic<float>* preGainParameter;
    std::atomic<float>* postGainParameter;
    
    std::atomic<float>* lfoFrequencyParameter;
    std::atomic<float>* lfoOutput;

    std::unique_ptr<DistortionEngine> distortionEngine;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MorphShaperAudioProcessor)
};
