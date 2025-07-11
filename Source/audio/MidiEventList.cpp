/*
  ==============================================================================

    MidiEventList.cpp
    Created: 23 Jul 2018
    Author:  Corey Ford

  ==============================================================================
*/

#include "MidiEventList.h"

namespace codetta
{
    MidiEventList::MidiEventList()
    {
        eventList.clear(); // ensure list is empty
    }
    
    MidiEventList::~MidiEventList(){}
    
    void MidiEventList::addMidiEvent (const MidiMessage& midiMessage)
    {
        eventList.add (midiMessage);
        eventList.sort (sorter); // sort by timestamp
    }
    
    void MidiEventList::removeMidiEvent (const MidiMessage& midiMessage)
    {
        for (int i = 0; i < eventList.size(); i++)
        {
            if (eventList[i] == midiMessage)
            {
                eventList.remove (i);
                break;  // leave the list
            }
        }
        
        eventList.sort (sorter); // sort by timestamp
    }
    
    MidiMessage MidiEventList::getMidiEvent (const int index) const
    {
        // the index you are getitng is out of range!!!
        jassert (index < getSize());
        
        return eventList[index];
    }
    
    MidiMessage MidiEventList::getLastEvent() const
    {
        return eventList.getLast();
    }
    
    void MidiEventList::clearAllEvents()
    {
        eventList.clear();
    }
    
    void MidiEventList::setMidiEvent (const int index,
                                      const MidiMessage& midiMessage)
    {
        // the index you want to set is out of range!!!
        jassert (index < getSize());
        
        eventList.set (index, midiMessage);
    }
    
    int MidiEventList::getSize() const
    {
        return eventList.size();
    }

    void MidiEventList::exportToFile(File file)
    {
        MidiFile midiFile;
        midiFile.setTicksPerQuarterNote(PlaybackSettings::get().getGlobalTempo() * 4);
        
        HashMap<int, int> gmMidiMapping; //< follow the GM MIDI standard
        gmMidiMapping.set(1, 1);
        gmMidiMapping.set(2, 10);
        gmMidiMapping.set(6, 41);
        gmMidiMapping.set(7, 42);
        gmMidiMapping.set(8, 43);
        gmMidiMapping.set(9, 57);
        gmMidiMapping.set(15, 94);
        
        // For the different channels
        for(int channel : {1,2,6,7,8,9,15})
        {
            // Create a new sequence
            MidiMessageSequence midiMessageSequence;
            
            // assign the correct instrument
            midiMessageSequence.addEvent(MidiMessage::programChange(channel, gmMidiMapping[channel] - 1));
            
            // for all the events
            for (int i = 0; i < getSize(); ++i){
                if(eventList[i].getChannel() == channel){
                    midiMessageSequence.addEvent(eventList[i], 0);
                }
            }
            
            //add as a track
            midiFile.addTrack(midiMessageSequence);
        }
        
        // track based on the general midi standard
//        midiMessageSequence.addEvent(MidiMessage::programChange(1, 1));
//        midiMessageSequence.addEvent(MidiMessage::programChange(2, 10));
//        midiMessageSequence.addEvent(MidiMessage::programChange(6, 41));
        
        // Output file
        FileOutputStream fileOutputStream (file);
        jassert(midiFile.writeTo(fileOutputStream));
        fileOutputStream.flush();
    }
    
    void MidiEventList::printEvents() const
    {
        DBG ("====== Events ======");
        for (int i = 0; i < getSize(); ++i)
        {
            DBG (eventList[i].getTimeStamp() << " - " << eventList[i].getNoteNumber() <<
                 (eventList[i].isNoteOn() ? " - on" : " - off"));
        }
        DBG ("====================");
    }
    
} // namespace codetta
