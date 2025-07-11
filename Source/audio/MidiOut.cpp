/*
  ==============================================================================

    MidiOut.cpp
    Created: 23 Oct 2018 8:52:27pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "MidiOut.h"

/** Main namespace for codetta. */
namespace codetta
{
    MidiOut::MidiOut (SoundfontAudioSource& audioSourceRhs) : audioSource (audioSourceRhs)
    {
        isPlaying = false;
        playhead = 0;
        timeStart = 0;
        listener = nullptr;
    }
    
    MidiOut::~MidiOut(){}
    
    void MidiOut::startPlaying()
    {
        // If you have hit this have tried to start playing whilst
        // playback is occouring. This can't be done as it could lead to
        // really horrible threading issues!!!
        jassert (isPlaying == false);
        
        // setup for playback
        isPlaying = true;
        isStopping = false;
        playhead = 0;
        timeStart = Time::getMillisecondCounterHiRes();
        
        // start the timer
        startTimer (1);
    }
    
    void MidiOut::timerCallback()
    {
        Array<MidiMessage> playback; //< array to store to be played values at this time

        // calculate how much time has elapsed
        double elapsedTime = Time::getMillisecondCounterHiRes() - timeStart;
        
        // get the current event at the playhead positiondouble
        double currentEventTimestamp = 0;
        if (playhead < MidiEventList::get().getSize())
            currentEventTimestamp = MidiEventList::get().getMidiEvent (playhead).getTimeStamp();
        
        // if the correct amount of time has elapsed...
        if (elapsedTime >= currentEventTimestamp || isStopping == true)
        {
            // from the current event up to any subsequent
            while (playhead < MidiEventList::get().getSize())
            {
                auto nextEventTimestamp = MidiEventList::get().getMidiEvent (playhead).getTimeStamp();
                
                // if this subsequent event is to be played at the same time as the current
                if (nextEventTimestamp == currentEventTimestamp)
                {
                    // add it to the queue
                    playback.add (MidiEventList::get().getMidiEvent (playhead));
                    playhead++;
                }
                else
                {
                    break; // leave the loop
                }
            }
        }
        
        // fire out midi events
        if (isStopping == false)
        {
            for (auto events : playback)
            {
                audioSource.processMidi (events);
            }
        }

        // clear playback
        playback.clear();
        
        // if playhead is at the end of playback
        if (playhead >= MidiEventList::get().getSize())
        {
            // stop playback thread
            isPlaying = false;
            isStopping = true;
            playhead = 0;
            stopTimer();
            
            if (listener != nullptr)
                listener->onStoppedPlaying();
        }
        
    }
    
    void MidiOut::addListener (Listener* newListener)
    {
        listener = newListener;
    }
    
    void MidiOut::setIsStopping (bool isStoppingRhs)
    {
        isStopping = isStoppingRhs;
        
        // turn off all channels off midi
        for (int i = 1; i <= 16; ++i)
        {
            for (int j = 0; j < 127; ++j)
                audioSource.processMidi (MidiMessage::noteOff (i, j));
        }
    }

} // namespace codetta
