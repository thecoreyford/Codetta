/*
  ==============================================================================

    MidiOut.h
    Created: 23 Oct 2018 8:52:27pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiEventList.h"
#include "CodettaAudioSource.h"
#include "../gui/LiveGuiPlayback.h"

#include "../libs/Amys-Notation/DrawComponent.h"
#include "../libs/Amys-Notation/BlockComponent.h"

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Class handling triggering midi output, and communicating with the playback gui.
     */
    class MidiOut    : public Timer
    {
    public:
        /**
         *  Constructor.
         *  @param Reference to the audio source to be used.
         */
        MidiOut (CodettaAudioSource& audioSourceRhs);
        
        /** Destructor. */
        ~MidiOut();
        
        /** Initialisation before timer callback is triggered. */
        void startPlaying();
        
        /** Triggers output of midi message.  */
        void timerCallback() override;
        
        /**
         *  Quick fire midi Message
         *  @param Midi message to be performed.
         */
        void quickFireMidi(MidiMessage message);
        
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
        
        void doArcs (const double& elapsedTime);
        
    private:
        /** Private constructor. */
        MidiOut();
        
        /** Currently playing? */
        bool isPlaying;
        /** Currently stopping? */
        bool isStopping;
        
        /** Reference to the audio source being used. */
        CodettaAudioSource& audioSource;
        
        /** Position of the playhead and arc head */
        int playhead, arcHead;
        
        /** Time playback started. */
        double timeStart;
        
        /** Pointer to the listener object. */
        Listener* listener;
    };
} // namespace codetta
