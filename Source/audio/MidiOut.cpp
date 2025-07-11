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
    MidiOut::MidiOut (CodettaAudioSource& audioSourceRhs) : audioSource (audioSourceRhs)
    {
        isPlaying = false;
        playhead = arcHead = 0;
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
        arcHead = 0;
        timeStart = Time::getMillisecondCounterHiRes();
        
        // start the timer
        startTimerHz (1024);
    }

    void MidiOut::doArcs(const double& elapsedTime)
    {
        auto arcs = LiveGuiPlayback::get().getTimestampedArcList();
        if (elapsedTime >= arcs[arcHead].timestamp)
        {
            // run away if all arcs have been done
            if (arcHead < arcs.size())
            {
                // load up all the arcs
                Array<Arcs::ArcInfo> stackedArcs;
                auto currentArc = arcs[arcHead].timestamp;
                stackedArcs.add(arcs[arcHead].info);
                
                while (arcs[arcHead++].timestamp == currentArc)
                    stackedArcs.add(arcs[arcHead].info);
                
                //fire out arcs
                if (isStopping == false)
                    for (auto i : stackedArcs)
                        Arcs::get().triggerArc (i.y, i.startX, i.endX);
            }
        }
    }
    
    void MidiOut::timerCallback()
    {
        Array<MidiMessage> playback; //< array to store to be played values at this time
        Array<MusiSyncEng::Note*> notes; //< array to store the notes to alter the gui
        Array<bool> noteOnGui;
        Array<PainterlyPlayback> drawNotes; //< array to store the notes for Amy's drawing blocks
        Array<PianoRollPlayback> pianoNotes; //< array to store the notes for Amy's piano roll esque blocks
        
        // calculate how much time has elapsed
        double elapsedTime = Time::getMillisecondCounterHiRes() - timeStart;
        
//        doArcs (elapsedTime); //< TODO: This slows down playback significantly
                                // can something smarter be done here?
        
        // Both lists should be the same size, with matching timestamps!
        jassert (notes.size() == playback.size());
        jassert (drawNotes.size() == playback.size());
        
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
                    notes.add (LiveGuiPlayback::get().getNote (playhead));
                    noteOnGui.add (LiveGuiPlayback::get().isNoteOn (playhead));
                    drawNotes.add (LiveGuiPlayback::get().getPaintNote (playhead));
                    pianoNotes.add (LiveGuiPlayback::get().getPianoRollBlock (playhead));
                    
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
            // These should match up and be the same size! Wahoo!
            jassert (notes.size() == playback.size());
            
            for (int i = 0; i < playback.size(); ++i)
            {
                // fire out the audio stuff
                audioSource.processMIDI (playback[i]);
                
                // do the live gui stuff too...
                if (notes[i] != nullptr /** for dummy notes */)
                {
                    if (noteOnGui[i] == true)
                        notes[i]->setCurrentColour (Colours::red);
                    
                    if (noteOnGui[i] == false)
                        notes[i]->setCurrentColour (Colours::black);
                }
                
                // do the live gui for the painterly stuff too...
                //TODO: broken when working in tandem with notes, but can inspect later...
                if (drawNotes[i].component != nullptr /** for dummy notes */)
                {
                    drawNotes[i].component->setRectDrawing (drawNotes[i].isNoteOn, drawNotes[i].index);
                }
                
                // do the live gui stuff for the piano blocks too...
                if (pianoNotes[i].index != -1 && pianoNotes[i].component != nullptr)
                {
                    pianoNotes[i].component->highlightButtonOnPlay(pianoNotes[i].index, pianoNotes[i].isNoteOn);
                }
                
            }
        }

        // clear playback and notes
        playback.clear();
        notes.clear();
        drawNotes.clear();
        pianoNotes.clear();
        
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
            
            // make all notes black again
            LiveGuiPlayback::get().setAllNotesBlack();
            
            // clear arcs
//            Arcs::get().stopDrawing();
        }
        
    }

    void MidiOut::quickFireMidi(MidiMessage message)
    {
        if (!isTimerRunning())
            audioSource.processMIDI (message);
    }

    
    void MidiOut::addListener (Listener* newListener)
    {
        listener = newListener;
    }
    
    void MidiOut::setIsStopping (bool isStoppingRhs)
    {
        isStopping = isStoppingRhs;
        
        // turn off all channels off midi
        for (int i = 1; i < PlaybackSettings::Instrument::total; ++i)
        {
            audioSource.processMIDI (MidiMessage::allNotesOff (i));
        }
        
        // think will ensure that there are not any bugs using the loading menu!
        // ... only lasts for scope anyway
//        MessageManagerLock mml (Thread::getCurrentThread());
        
        // make all notes black again
//        LiveGuiPlayback::get().setAllNotesBlack();
        
        // clear arcs
//        Arcs::get().stopDrawing();
    }

} // namespace codetta
