/*
  ==============================================================================

    PlaybackSettings.h
    Created: 22 Oct 2018 9:23:45pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../libs/MusicSync-Notation-Engine/Font.h"
#include "MidiEventList.h"

#include <stack>
#include <set>
#include "../libs/juckly/client/Block.h"
#include "../libs/MusicSync-Notation-Engine/OffsetMappings.h"

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Singleton handling all the global state changes that happen
     *  when queueing @see MidiEventList.h.
     */
    class PlaybackSettings
    {
    public:
        /**
         *  Acessor function for the singleton settings.
         *  @return reference to the playback class.
         */
        static PlaybackSettings& get()
        {
            static PlaybackSettings instance;
            return instance;
        }
        
        /**
         *  Setter for the beats per min.
         *  @param the new bpm value.
         */
        void setBPM (int newBPM);

        /**
         *  Getter for the BPM
         *  @return the current bpm value.
         */
        int getBPM() const;
        
        /**
         *  Setter for the latest timestamp addition.
         *  @param new timestamp value.
         */
        void setLatestTimestampAddition (int latestAddition);
        
        /**
         *  Getter for the latest timestamp addition.
         *  @return the latest timestamp value added.
         */
        int getLatestTimestampAddition () const;
        
        /**
         *  Calculation of the length of a note in milliseconds.
         *  @param the note that a value will be be calculated.
         *  @return the note length in milliseconds
         */
        int lengthOfNoteInMillisec (MusiSyncEng::NewFont::NoteValue note);
        
        /**
         *  Calculation of the length of a note in milliseconds.
         *  @param the note value as a float.
         *  @return the note length in milliseconds
         */
        int lengthOfNoteInMillisec (float noteValue);
        
        /**
         *  Push the start repeat block to its stack.
         *  @param the repeat block to be pushed.
         */
        void registerStartRepeat (juckly::Block* startRepeat);
        
        /**
         *  Setter for the global tempo blocks state.
         *  The new tempo for the global tempo.
         *  @param the new global tempo value.
         */
        void setGlobalTempo (int newTempo);
        
        /**
         *  Getter for the global tempo blocks state.
         *  @return the registration state for the global tempo.
         */
        int getGlobalTempo() const;
        
        /**
         *  Getter for the repeat block at the top of the stack.
         *  @returns the repeat block at the top of the stack, or nullptr if
         *      the stack is empty.
         */
        juckly::Block* getThisRepeat() const;
        
        /**
         *  Pops the top item from the repeat block stack.
         */
        void deregisterStartRepeat();
        
        /**
         * Pops all items from the start repeat stack.
         */
        void clearAllStartRepeats();
        
        //======================================================================
        
        /**
         *  Setter for the current clef type.
         *  @param the new clef type.
         */
        void setCurrentClef (MusiSyncEng::ClefTypes newClef);
        
        /**
         *  Getter for the current clef type.
         *  @return the current clef type enumerated value.
         */
        MusiSyncEng::ClefTypes getCurrentClef() const;
        
        //======================================================================
        
        /** Enumeration of PC message for the different instruments*/
        enum Instrument
        {
            piano = 1,
            glockenspiel = 2,
            acousticGuitar = 3,
            distortionGuitar = 4,
            electricBass = 5,
            violin = 6,
            viola = 7,
            cello = 8,
            trumpet = 9,
            drums = 10,
            altoSax = 11,
            tenorSax = 12,
            basoon = 13,
            flute = 14,
            pad = 15,
            total = 16
        };
        
        /**
         *  Setter for the current instrument.
         *  @param the new instrument .
         */
        void setInstrument (Instrument instrument);
        
        /**
         *  Getter for the current instrument.
         *  @return the new current instument.
         */
        Instrument getInstrument() const;
        
        //======================================================================
        
        /**
         *  Setter for the current velocity.
         *  @param the new velocity value.
         */
        void setVelocity (int newVelocity);
        
        /**
         *  Getter for the current velocity.
         *  @return the current velocity.
         */
        int getVelocity() const;
        
        //======================================================================
        
        /**
         *  Setter for the current tuning offset.
         *  @param the new tuning offset value.
         */
        void setTuningOffset (int newTuningOffset);
        
        /**
         *  Getter for the current tuning offset.
         *  @return the current tuning offset.
         */
        int getTuningOffset() const;
        
        int getTuningOffsetCMajorMIDIValue (int midiValueToOffset)
        {
            MusiSyncEng::OffsetMappings map;
            auto scale = map.getMajorScale();
            
            // The note is not in the scale! Oh NO!
            jassert (scale.contains (midiValueToOffset));

            return scale.getUnchecked (scale.indexOf (midiValueToOffset) + tuningOffset);
        }
        
    private:
        /** Private constructor. */
        PlaybackSettings();
        
        /** Private Destructor */
        ~PlaybackSettings(){}
        
        /**
         *  Private Copy Constructor
         *  @param copyable value
         */
        PlaybackSettings (const PlaybackSettings&){}
        
        /**
         *  Private Copy Operator
         *  @param copyable value
         */
        void operator= (const PlaybackSettings&){}
        
        //======================================================================
        
        /** Current tempo and velocity for playback */
        int bpm, globalBPM, velocity, tuningOffset;
        
        /** The last timestamp added to a sequence. */
        float latestTimestampAddition;
        
        /** Stack holding each start block event.*/
        std::stack<juckly::Block*> lastLoop;
        
        /** We will also keep track of the loops for easy searching */
        Array<juckly::Block*> lastLoopTracker;
        
        /** Current clef mapping to be used. */
        MusiSyncEng::ClefTypes currentClef;
        
        //======================================================================
                
        /** Current instrument to be used. */
        Instrument instrument;
        
    };
} // namespace codetta
