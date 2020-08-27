/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DirtyBoyAudioProcessorEditor::DirtyBoyAudioProcessorEditor (DirtyBoyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    LookAndFeel::setDefaultLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(mImageComponent);

    version.setFont(customLookAndFeel.setCustomFontAndSize(20.0f));
    version.setColour(juce::Label::textColourId, Colour(0xFF8BFFBC));
    version.setText("V1.0", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(version);

    driveValue = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DRIVE_ID, driveSlider);
    driveSlider.setSliderStyle(juce::Slider::Rotary);
    driveSlider.setRotaryParameters(juce::MathConstants<float>::pi * 1.2f, juce::MathConstants<float>::pi * 2.8f, false);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    driveSlider.setTextValueSuffix(" %");
    driveSlider.setDoubleClickReturnValue(true, 0.0); // double-clicking this slider will set it to 0
    driveSlider.setRange(0, 100, 1);
    //driveSlider.setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(driveSlider);

    phatValue = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, PHAT_ID, phatSlider);
    phatSlider.setSliderStyle(juce::Slider::Rotary);
    phatSlider.setRotaryParameters(juce::MathConstants<float>::pi * 1.2f, juce::MathConstants<float>::pi * 2.8f, false);
    phatSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    phatSlider.setTextValueSuffix(" %");
    phatSlider.setDoubleClickReturnValue(true, 0.0); // double-clicking this slider will set it to 0
    phatSlider.setRange(0, 100, 1);
    //phatSlider.setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(phatSlider);

    crushValue = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, CRUSH_ID, crushSlider);
    crushSlider.setSliderStyle(juce::Slider::Rotary);
    crushSlider.setRotaryParameters(juce::MathConstants<float>::pi * 1.2f, juce::MathConstants<float>::pi * 2.8f, false);
    crushSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    crushSlider.setTextValueSuffix(" %");
    crushSlider.setDoubleClickReturnValue(true, 0.0); // double-clicking this slider will set it to 0
    crushSlider.setRange(0, 100, 1);
    //crushSlider.setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(crushSlider);

    volumeValue = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, VOLUME_ID, volumeSlider);
    volumeSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    volumeSlider.setColour(0, juce::Colours::white);
    volumeSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    volumeSlider.setDoubleClickReturnValue(true, 0.0); // double-clicking this slider will set it to 0
    //volumeSlider.setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(volumeSlider);

    linkLabel.setFont(customLookAndFeel.setCustomFontAndSize(24.0f), false, Justification::centred);
    linkLabel.setAlwaysOnTop(true);
    linkLabel.setColour(juce::HyperlinkButton::textColourId, juce::Colours::white);
    linkLabel.setButtonText("ABOUT");
    linkLabel.setURL(juce::URL("https://github.com/ajsprojects/DirtyBoy"));
    addAndMakeVisible(linkLabel);

    autoGainButton.setToggleState(true, false);
    autoGainButton.setClickingTogglesState(true);
    autoGainButton.addListener(this);
    addAndMakeVisible(autoGainButton);

    setSize(360, 340);
}

void DirtyBoyAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == &autoGainButton && autoGainButton.getToggleState()) {
        audioProcessor.autoGain = true;
    }
    if (button == &autoGainButton && !autoGainButton.getToggleState()) {
        audioProcessor.autoGain = false;
    }
}

DirtyBoyAudioProcessorEditor::~DirtyBoyAudioProcessorEditor()
{
}

//==============================================================================
void DirtyBoyAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    
    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::dirtyboy_png, BinaryData::dirtyboy_pngSize);
    g.drawImageAt(background, 0, 0);
}

void DirtyBoyAudioProcessorEditor::resized()
{
    driveSlider.setBounds(150, 85, 110, 110);
    phatSlider.setBounds(135, 150, 110, 110);
    crushSlider.setBounds(170, 215, 110, 110);
    volumeSlider.setBounds(320, 120, 15, 145);
    linkLabel.setBounds(120, 310, 140, 35);
    autoGainButton.setBounds(274, 293, 20, 17);
    version.setBounds(290, 72, 50, 15);
}
