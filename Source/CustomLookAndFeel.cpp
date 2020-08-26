#pragma once
#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
	setColour(juce::Slider::thumbColourId, juce::Colours::floralwhite);
	setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
	setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey);
	setColour(Slider::trackColourId, juce::Colours::white);
}

const Font CustomLookAndFeel::setCustomFontAndSize(float size)
{
	static auto typeface = Typeface::createSystemTypefaceFor(BinaryData::raavi_ttf, BinaryData::raavi_ttfSize);
	Font font = typeface;
	font.setHeight(size);
	return font;
}

const Font CustomLookAndFeel::loadCustomFont()
{
	static auto typeface = Typeface::createSystemTypefaceFor(BinaryData::raavi_ttf, BinaryData::raavi_ttfSize);
	Font font = typeface;
	return font;
}

Typeface::Ptr CustomLookAndFeel::getTypefaceForFont()
{
	return loadCustomFont().getTypeface();
}

Font CustomLookAndFeel::getSliderPopupFont(Slider&)
{
	return Font(loadCustomFont());
}

