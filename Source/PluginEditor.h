/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/**
*/
class DirtyBoyAudioProcessorEditor  : public juce::AudioProcessorEditor, public ToggleButton::Listener
{
public:
    DirtyBoyAudioProcessorEditor (DirtyBoyAudioProcessor&);
    ~DirtyBoyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> driveValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> phatValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> crushValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> volumeValue;

    void buttonClicked(Button* button) override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DirtyBoyAudioProcessor& audioProcessor;

    ImageComponent mImageComponent;
    Slider driveSlider;
    Slider phatSlider;
    Slider crushSlider;
    Slider volumeSlider;
    HyperlinkButton linkLabel; 
    ToggleButton autoGainButton;
    CustomLookAndFeel customLookAndFeel;
    Label version;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirtyBoyAudioProcessorEditor)
};
