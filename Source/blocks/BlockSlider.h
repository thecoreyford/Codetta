/*
 ==============================================================================
 
 BlockSlider.h
 Created: 30 Nov 2018 7:09:31pm
 Author:  Corey Ford
 
 ==============================================================================
 */

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../libs/MusicSync-Notation-Engine/Font.h"

//==============================================================================

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Slider componet with a crotchet icon.
     */
    class BlockSlider    : public Component
    {
    public:
        /**
         *  Constructor.
         *  @param text is the label leftmost of the slider value.
         *  @param startValue is the lowest value for the sliders range.
         *  @param endvalue is the highest value for the sliders range.
         *  @param defaultValue is the starting value for the slider.
         *  @param scaleTextRhs will multiply the font sizing for the text.
         *  @param yOffsetRhs will move the text up and down in position.
         *  @param xOffsetRhs will move the text left and right in position.
         *  @param usesMusicFontRhs will use the MusiSync font if true.
         */
        BlockSlider (String text,
                     double startValue,
                     double endValue,
                     double defaultValue,
                     float scaleTextRhs = 1.0,
                     int yOffsetRhs = 0,
                     int xOffsetRhs = 0,
                     const bool usesMusicFontRhs = true
                     )
            : slider (Slider::SliderStyle::IncDecButtons, Slider::TextEntryBoxPosition::TextBoxBelow),
              textLabel (text), scaleText (scaleTextRhs), yOffset (yOffsetRhs), xOffset (xOffsetRhs),
              usesMusicFont (usesMusicFontRhs)
        {
            addAndMakeVisible (slider);
            slider.setRange (startValue, endValue, 1.0);
            slider.setValue (defaultValue);
            slider.setColour (Slider::ColourIds::textBoxTextColourId,
                              Colours::black);
        }
        
        /** Destructor */
        ~BlockSlider() { }
        
        /**
         * Paints background for the object & crotchet font.
         * @param the graphics context.
         */
        void paint (Graphics& g) override
        {
            if (usesMusicFont)
                g.setFont (MusiSyncEng::NewFont::getNotationFont().withHeight (getHeight() * scaleText));
            
            g.setColour (Colours::black);
            g.drawText (textLabel, xOffset, yOffset, getWidth() * 0.50, getHeight(),
                        Justification::left, false);
        }
        
        /** Sets slider bounds */
        void resized() override
        {
            slider.setBounds (getLocalBounds().withTrimmedLeft(getWidth() * 0.50));
        }
        
        /**
         *  Getter for the sliders value;
         *  @return the slider value cast to an integer.
         */
        int getValue()
        {
            return (int) slider.getValue();
        }
        
    private:
        /** The slider used to select values from */
        Slider slider;
        /** The text to be displayed next to the slider */
        String textLabel;
        /** The scaling for the text */
        float scaleText;
        /** The offset for the texts y position */
        int yOffset;
        /** The offset for the texts x position */
        int xOffset;
        /** True if using the MusiSync Font */
        const bool usesMusicFont;
    };

} // namespace codetta

