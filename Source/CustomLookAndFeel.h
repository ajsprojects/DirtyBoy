#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class CustomLookAndFeel : public LookAndFeel_V4
{
public:
	CustomLookAndFeel();
	const Font setCustomFontAndSize(float size);
	const Font loadCustomFont();
	Typeface::Ptr getTypefaceForFont();
	Font getSliderPopupFont(Slider&) override;
};
