/*
  ==============================================================================

    MidiOut.h
    Created: 23 Oct 2018 8:52:27pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../3rdparty/juce-soundfonts/Source/SoundfontAudioSource.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiEventList.h"

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Class handling triggering midi output.
     */
    class MidiOut    : public Timer
    {
    public:
        /**
         *  Constructor.
         *  @param Reference to the audio source to be used.
         */
        MidiOut (SoundfontAudioSource& audioSourceRhs);
        
        /** Destructor. */
        ~MidiOut();
        
        /** Initialisation before timer callback is triggered. */
        void startPlaying();
        
        /** Triggers output of midi message.  */
        void timerCallback() override;
        
        /**
         *  Listener class for when midi output is completed.
         */
        class Listener
        {
        public:
            /** Destructor. */
            virtual ~Listener(){}
            
            /** Callback for when midi output is finished. */
            virtual void onStoppedPlaying() = 0;
        };
        
        /**
         *  Setter for midi output listener objects.
         *  @param adress of the new listener.
         */
        void addListener (Listener* newListener);
        
        //======================================================================
        
        /**
         *  Change state of stopping.
         *  @param the new state if stopping.
         */
        void setIsStopping (bool isStoppingRhs);
        
    private:
        /** Private constructor. */
        MidiOut();
        
        /** Currently playing? */
        bool isPlaying;
        /** Currently stopping? */
        bool isStopping;
        
        /** Reference to the audio source being used. */
        SoundfontAudioSource& audioSource;
        
        /** Position of the playhead. */
        int playhead;
        
        /** Time playback started. */
        double timeStart;
        
        /** Pointer to the listener object. */
        Listener* listener;
    };
} // namespace codetta
