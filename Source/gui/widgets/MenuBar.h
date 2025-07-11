/*
  ==============================================================================

    MenuBar.h
    Created: 14 Oct 2019 12:52:53pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Widget containing the gui for the menu bar object.
     */
    class MenuBar    : public Component
    {
    public:
        /** Constructor */
        MenuBar();
        
        /** Destructor */
        ~MenuBar();

        /**
         *  Paint.
         *  @param Graphics context for this object.
         */
        void paint (Graphics&) override;
        
        /** Reszied function for bounds */
        void resized() override;
        
        /**
         * Class containing the look and feel for menu bar buttons.
         */
        class MenuButtonLookAndFeel : public LookAndFeel_V4
        {
            /**
             *  Drawing code for the button design.
             *  @param g is the graphics context for the button
             *  @param button is the reference to the button with the look and feel
             *  @param true if mouse if over button
             *  @param true if mouse if down
             */
            void drawButtonBackground (Graphics& g,
                                       Button& button,
                                       const Colour& bgColour,
                                       bool isMouseOverButton,
                                       bool isMouseDown) override;
        };
        
        class Listener
        {
        public:
            virtual ~Listener(){}
            virtual void menuBarPressed (const String& item) = 0;
        };

        void addListener (Listener* newListener) {listener = newListener;}
    
    private:
        /** File button. */
        ImageButton fileButton;
        
//        * Look and feel for all buttons added to the menu
//        MenuButtonLookAndFeel lookAndFeel;
        
        Listener* listener;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuBar)
    };
} //namespace codetta

