/*
  ==============================================================================

    PlayBackControls.h
    Created: 8 Nov 2018 12:37:25pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "InfoBar.h"

//==============================================================================
/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Playback Controller Widget
     */
    class PlayBackControls    : public Component
    {
    public:
        /** Constructor. */
        PlayBackControls();
        /** Destructor. */
        ~PlayBackControls();
        
        /**
         * Paints background for the object.
         * @param the graphics context.
         */
        void paint (Graphics&) override;
        
        /** Sets button bounds */
        void resized() override;
        
        //======================================================================
        
        /** Listener for the playback controller buttons. */
        class Listener
        {
        public:
            /** Destructor */
            virtual ~Listener(){}
            
            /**
             *  Callback for when play is clicked.
             *  @param Reference to the play button.
             *  @param Reference to the stop button.
             */
            virtual void onPlayClicked (TextButton& play, TextButton& stop) = 0;
            
            /**
             *  Callback for when stop is clicked.
             *  @param Reference to the play button.
             *  @param Reference to the stop button.
             */
            virtual void onStopClicked (TextButton& play, TextButton& stop) = 0;
        };
        
        /**
         *  Setters the button listener
         *  @param the new listener
         */
        void addListener (Listener* newListener);
        
        /** Actions to be performed when playing is stopped. */
        void onPlayingStopped();
        
        //======================================================================
        
    private:
        /** Button to start playback. */
        TextButton play;
        
        /** Button to stop playback. */
        TextButton stop;
        
        /** The listener object for this class */
        Listener* listener;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayBackControls)
    };

} // namespace codetta
