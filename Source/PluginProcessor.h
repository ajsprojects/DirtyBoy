/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#define DRIVE_ID "drive"
#define DRIVE_NAME "Drive"
#define PHAT_ID "phat"
#define PHAT_NAME "Phat"
#define CRUSH_ID "crush"
#define CRUSH_NAME "Crush"
#define VOLUME_ID "volume"
#define VOLUME_NAME "Volume"
//==============================================================================
/**
*/
class DirtyBoyAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DirtyBoyAudioProcessor();
    ~DirtyBoyAudioProcessor() override;

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

    AudioProcessorValueTreeState treeState;
    bool autoGain;

private:
    //==============================================================================
    dsp::ProcessorDuplicator <dsp::IIR::Filter<float>, dsp::IIR::Coefficients <float>> lowPassFilter;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirtyBoyAudioProcessor)
};
