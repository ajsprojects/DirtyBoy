/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DirtyBoyAudioProcessor::DirtyBoyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    treeState(*this, nullptr, "PARAMETERS",
        { //MIN, MAX , DEFAULT
        std::make_unique<juce::AudioParameterInt>(DRIVE_ID, DRIVE_NAME, 0, 100, 0),
        std::make_unique<juce::AudioParameterInt>(PHAT_ID, PHAT_NAME, 0, 100, 0),
        std::make_unique<juce::AudioParameterInt>(CRUSH_ID , CRUSH_NAME , 0, 100, 0),
        std::make_unique<juce::AudioParameterFloat>(VOLUME_ID, VOLUME_NAME, NormalisableRange<float>(-36.0,10,0.1),0.0)
    })
#endif
{
    treeState.state = juce::ValueTree("savedParams");
}

DirtyBoyAudioProcessor::~DirtyBoyAudioProcessor()
{
}

//==============================================================================
const juce::String DirtyBoyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DirtyBoyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DirtyBoyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DirtyBoyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DirtyBoyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DirtyBoyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DirtyBoyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DirtyBoyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DirtyBoyAudioProcessor::getProgramName (int index)
{
    return {};
}

void DirtyBoyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DirtyBoyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  
}

void DirtyBoyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DirtyBoyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void DirtyBoyAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Dont need to keep this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    float VOLUME = *treeState.getRawParameterValue(VOLUME_ID);
    float DRIVE = *treeState.getRawParameterValue(DRIVE_ID) / 10;
    float PHAT = *treeState.getRawParameterValue(PHAT_ID);
    float CRUSH = *treeState.getRawParameterValue(CRUSH_ID) / 100;

    float inputLevel = buffer.getRMSLevel(0, 0, buffer.getNumSamples()) + buffer.getRMSLevel(1, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
      
        //(HARD CLIP)   
        if (DRIVE > 0)
        {
            for (int i = 0; i < buffer.getNumSamples(); ++i) {

                // processedSample = ((channelData[i] > 0) ? 1 : -1) * (1 - std::exp(-std::abs(channelData[i] * phat)));
                 //channelData[i] = processedSample * phat + channelData[i] * (1.0 - phat);
                channelData[i] = std::tanh(channelData[i] * (2 + DRIVE));// + processedSample;
            }

        }
        //HARMONIC WAVESHAPER
        if (PHAT > 0)
        {
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                channelData[i] = (1.0 + PHAT) * channelData[i] / (1.0 + PHAT * abs(channelData[i]));
            }
        }
        //CRUSH/BITCRUSH
        if (CRUSH > 0)
        {
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                float dry = channelData[i];
                float wet = round(channelData[i] * pow(2, CRUSH)) / pow(4, CRUSH);
                //channelData[i] = (wet + dry); //+ (wet * LIFT) / (LIFT - 1);
                //channelData[i] = (wet + dry) + (wet * LIFT) / (LIFT - 1);
                channelData[i] = (wet * CRUSH) + std::tanh(dry);
            }
        }

        #if defined(_DEBUG)
            Logger::writeToLog(String::formatted("VOLUME: %f", VOLUME));
            Logger::writeToLog(String::formatted("DRIVE: %f", DRIVE));
            Logger::writeToLog(String::formatted("PHAT: %f", PHAT));
            Logger::writeToLog(String::formatted("CRUSH: %f", CRUSH));
        #endif  
    }
    float outputLevel = buffer.getRMSLevel(0, 0, buffer.getNumSamples()) + buffer.getRMSLevel(1, 0, buffer.getNumSamples());
    if (autoGain) //HELPS CONTROL LEVEL OF OUTPUT AUDIO
    {
        if (inputLevel != outputLevel);
        {
            float inputLevelDecibel = juce::Decibels::decibelsToGain(inputLevel) / 2;
            float outputLevelDecibel = juce::Decibels::decibelsToGain(outputLevel) / 2;
            buffer.applyGainRamp(0, buffer.getNumSamples(), outputLevelDecibel, inputLevelDecibel);
            Logger::writeToLog(String::formatted("Gain Input: %f | Gain Output: %f", inputLevelDecibel, outputLevelDecibel));
        } 
    }

    float outputGain = juce::Decibels::decibelsToGain(VOLUME, -36.0f);
    buffer.applyGain(outputGain);
    Logger::writeToLog(String::formatted("Applying Gain: %f", outputGain));
}

//==============================================================================
bool DirtyBoyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DirtyBoyAudioProcessor::createEditor()
{
    return new DirtyBoyAudioProcessorEditor (*this);
}

//==============================================================================
void DirtyBoyAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = treeState.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DirtyBoyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(treeState.state.getType()))
        {
            treeState.replaceState(ValueTree::fromXml(*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DirtyBoyAudioProcessor();
}
