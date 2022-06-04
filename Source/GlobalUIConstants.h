#pragma once
#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

namespace UI {
	namespace GLOBAL
	{
		const float scale = 1.0f;
    
        const int pluginWidth = 1000;
        const int pluginHeight = 700;

		const int sliderComponentWidth = 60;
		const int sliderComponentHeight = 60;
		const int sliderComponentPadding = 10;

		const int sliderTextBoxWidth = 50;
		const int sliderTextBoxHeight = 25;
    
        //const int sliderLabelWidth = 60;
        const int sliderLabelHeight = 25;

		const juce::Colour backColour(196, 196, 196);
		const juce::Colour strokeLineColour(0, 0, 0);

		const int strokeLineWigthInside = 2;
		const int strokeLineWigthOutside = 3;

		const int paddingFromStoke = 7;
		const int paddingComponentsInside = 7;
	}

	namespace ADSR
	{
		const int envelopeWidth = 400;

		const int movedDotDiameter = 15;
		const int movedDotAreaSize = 25;
		const juce::Colour dotColour(0, 0, 0);
		const juce::Colour dotMouseOnColour(128, 128, 128);
    
        const int lineThikness = 3;
        const juce::Colour lineColour(0, 0, 0);
	}

    namespace OSC_BLOCK{
        const int blockWidth = 500;
        const int blockHeight = 350;
    
        const int toggleWidth = 30;
        const int toggleHeight = 1;
    }

    namespace LFO_BLOCK{
        const int width = 500;
        const int height = 350;
    }

    namespace LFO{
        //const int blockWidth = 500;
        //const int blockHeight = 350;
    
        const int envWidth = 485;
        const int envHeight = 300;
    }

    namespace OSC_PROPERTIES{
        const int height = 110;
        const int buttonSize = 40;
    }

    namespace MODULATION_MATRIX{
        const int width = 500;
        const int height = 350;
    
        const int labelHeight = 40;
    }
}
