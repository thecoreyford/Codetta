/*
  ==============================================================================

    RectButton.h
    Created: 24 Jun 2019 4:31:54pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once
#include "../../../../JuceLibraryCode/JuceHeader.h"


/** Namespace for the MusiSync Notation Engine */
namespace MusiSyncEng
{
    
    class RectButtonLookAndFeel    : public LookAndFeel_V4
    {
    public:
        void drawButtonBackground (Graphics& g,
                                   Button& button,
                                   const Colour& backgroundColour,
                                   bool isMouseOverButton,
                                   bool isButtonDown) override
        {
            auto buttonArea = button.getLocalBounds().reduced (1);
            g.setColour (backgroundColour);
            g.fillRect (buttonArea);
        }
        
    private:
        
    };
    
    //=========================================================================
    
    class RectButton    : public TextButton
    {
    public:
        RectButton()
        {
            feel = std::make_unique<RectButtonLookAndFeel>();
            setLookAndFeel (feel.get());
        }
        
        ~RectButton()
        {
            setLookAndFeel (nullptr);
        }
        
    private:
        std::unique_ptr<RectButtonLookAndFeel> feel;
    };
    
} // namespace MusiSyncEng
