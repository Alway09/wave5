#pragma once
#include <JuceHeader.h>

namespace UI {
	namespace GLOBAL
	{
		const float scale = 1.0f;

		const int sliderComponentWidth = 100;
		const int sliderComponentHeight = 100;
		const int sliderComponentPadding = 10;

		const int sliderTextBoxWidth = 50;
		const int sliderTextBoxHeight = 25;

		const juce::Colour backColour(196, 196, 196);
		const juce::Colour strokeLineColour(0, 0, 0);

		const int strokeLineWigthInside = 2;
		const int strokeLineWigthOutside = 3;

		const int paddingFromStoke = 7;
		const int paddingComponentsInside = 10;
	}

	namespace ADSR
	{
		const int envelopeWidth = 400;

		const int movedDotDiameter = 15;
		const int movedDotAreaSize = 25;
		const juce::Colour dotColour(0, 0, 0);
		const juce::Colour dotMouseOnColour(128, 128, 128);
	}
}
