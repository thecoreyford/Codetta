/*
  ==============================================================================

    PlaybackSettings.cpp
    Created: 11 Jan 2019 2:35:40pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "PlaybackSettings.h"

/** Main namespace for codetta. */
namespace codetta
{
    void PlaybackSettings::setBPM (int newBPM)
    {
        bpm = newBPM;
    }
    
    int PlaybackSettings::getBPM() const
    {
        return bpm;
    }
    
    void PlaybackSettings::setLatestTimestampAddition (int latestAddition)
    {
        latestTimestampAddition = latestAddition;
    }
    
    int PlaybackSettings::getLatestTimestampAddition() const
    {
        return latestTimestampAddition;
    }
    
    int PlaybackSettings::lengthOfNoteInMillisec (MusiSyncEng::NewFont::NoteValue note)
    {
        // REF: (Ford, 2016)
        return (int)(
                     60      /* seconds */
                     * 1000  /* milliseconds per second */
                     * 4     /* crotchets per semibreve */
                     * MusiSyncEng::NewFont::getNoteDivision (note)
                     / bpm
                     );
    }
    
    int PlaybackSettings::lengthOfNoteInMillisec (float noteValue)
    {
        // REF: (Ford, 2016)
        return (int)(
                     60      /* seconds */
                     * 1000  /* milliseconds per second */
                     * 4     /* crotchets per semibreve */
                     * noteValue
                     / bpm
                     );
    }
    
   
    void PlaybackSettings::registerStartRepeat (juckly::Block* startRepeat)
    {
        if (!lastLoopTracker.contains (startRepeat))
        {
            lastLoop.push (startRepeat);
            lastLoopTracker.add (startRepeat);
        }
    }
    
    void PlaybackSettings::setGlobalTempo (int newTempo)
    {
        globalBPM = newTempo;
    }
    
    int PlaybackSettings::getGlobalTempo() const
    {
        return globalBPM;
    }
    
    juckly::Block* PlaybackSettings::getThisRepeat() const
    {
        if (lastLoop.empty() == true)
            return nullptr;
        else
        {
            return lastLoop.top();
        }
    }
    
    void PlaybackSettings::deregisterStartRepeat()
    {
        if (lastLoop.empty() == false)
        {
            int index = lastLoopTracker.indexOf (lastLoop.top());
            lastLoopTracker.remove (index);
            lastLoop.pop();
        }
    }
    
    void PlaybackSettings::clearAllStartRepeats()
    {
        while (lastLoop.empty() == false)
            deregisterStartRepeat();
        
        lastLoopTracker.clear();
    }
    
    void PlaybackSettings::setCurrentClef (MusiSyncEng::ClefTypes newClef)
    {
        currentClef = newClef;
    }
    
    MusiSyncEng::ClefTypes PlaybackSettings::getCurrentClef() const
    {
        return currentClef;
    }
    
    //==========================================================================
    
    PlaybackSettings::PlaybackSettings()
    {
        // Set defaults
        currentClef = MusiSyncEng::ClefTypes::Treble;
        instrument = Instrument::piano;
        bpm = 120;
        globalBPM = 120;
        latestTimestampAddition = 0.0f;
        clearAllStartRepeats();
        velocity = 63;
    }
    
    //==========================================================================
    
    void PlaybackSettings::setInstrument (Instrument newInstrument)
    {
        instrument = newInstrument;
    }
    
    PlaybackSettings::Instrument PlaybackSettings::getInstrument() const
    {
        return instrument;
    }
    
    //==========================================================================

    void PlaybackSettings::setVelocity (int newVelocity)
    {
        velocity = newVelocity;
        if (velocity > 127.0f) velocity = 127.0f;
        if (velocity < 1.0f) velocity = 1.0f;
    }
    
    int PlaybackSettings::getVelocity() const
    {
        return velocity;
    }
    
} // namespace codetta
