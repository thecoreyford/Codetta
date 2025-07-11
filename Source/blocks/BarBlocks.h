/*
  ==============================================================================

    BarBlocks.h
    Created: 17 Sep 2018 12:25:23pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../libs/juckly/client/Block.h"
#include "../libs/juckly/client/BlockSettings.h"
#include "../libs/MusicSync-Notation-Engine/Bar.h"
#include "../libs/MusicSync-Notation-Engine/Font.h"
#include "../audio/Audio.h"

using FragmentPtr = std::unique_ptr<MusiSyncEng::NoteFragment>;

//==============================================================================

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Parent class for Codetta blocks containg a "bar" of music.
     */
    class BarBlock    : public juckly::Block
    {
    public:
        /**
         *  Constructor.
         *  @param Unique blockID name.
         *  @param Internal bar object from musicsync engine.
         *  @param Icon image for this bar block.
         *  @param Background image for this bar block.
         *  @param The width of the bar block (usually a multiple of 90)
         */
        BarBlock (String ID,
                  MusiSyncEng::Bar* timeSignature,
                  Image icon,
                  Image onScreen,
                  int x) : juckly::Block ( std::make_unique<juckly::CommandBlockSettings>
                                            (ID,
                                             icon,
                                             onScreen,
                                             false /* not a start node */,
                                             timeSignature,
                                             false /* dosen't take a parameter */,
                                             x
                                             )
                                            )
        {
        }
    protected:
        /**
         *  Function writing all notes of a bar the event list, with the correct
         *  timestamps.
         */
        void calculateBarTiming()
        {
            auto bar = getInternalUI();
            if (dynamic_cast<MusiSyncEng::Bar*>(bar) != nullptr)
            {
                // Cast was successfull
                auto& fragments = dynamic_cast<MusiSyncEng::Bar*>(bar)->getFragment();
                
                // Cycle through each fragment in a bar.
                for (int i = 0; i < fragments.size(); ++i)
                {
                    // timestamp must be offest by previous value
                    float timeStamp = PlaybackSettings::get().getLatestTimestampAddition();
                    
                    auto fragment = fragments[i]->getFirstNoteOfFragment();
                    auto noOfNotes = fragments[i]->getNoOfNotesInFragment();
                    // cycle though all notes in the fragment
                    for (int j = 0; j < noOfNotes; ++j)
                    {
                        // ensure that our offset mapping is correct
                        fragment[j]->changeClef (PlaybackSettings::get().getCurrentClef());
                        
                        //====================================================================================
                        // translate into message on
                        auto messageOn = MidiMessage::noteOn ((int)PlaybackSettings::get().getInstrument(),
                                                              fragment[j]->getMusicalValue(),
                                                              (uint8) PlaybackSettings::get().getVelocity());
                        messageOn.setTimeStamp (timeStamp); //< add timestamp
                        MidiEventList::get().addMidiEvent (messageOn); //< add to event list
                        
                        //====================================================================================
                        
                        // calculate the next value
                        timeStamp += PlaybackSettings::get().lengthOfNoteInMillisec (fragment[j]->getValue());
                        
                        //timeStamp += PlaybackSettings::get().getLatestTimestampAddition();
                        
                        // add note off
                        auto messageOff = MidiMessage::noteOff ((int)PlaybackSettings::get().getInstrument(),
                                                                fragment[j]->getMusicalValue(),
                                                                (uint8) PlaybackSettings::get().getVelocity());
                        messageOff.setTimeStamp (timeStamp); //< add timestamp
                        MidiEventList::get().addMidiEvent (messageOff); // add to event list
                        
                        // update latest addition
                        PlaybackSettings::get().setLatestTimestampAddition (timeStamp);
                        
                    }
                }
                
                //===========================================================================================
                
                // add any remaining space
                auto& valueLeft = dynamic_cast<MusiSyncEng::Bar*>(bar)->getValueLeft();
                if (valueLeft > 0)
                {
                    // timestamp must be offest by previous value
                    float timeStamp = PlaybackSettings::get().getLatestTimestampAddition();
                    
                    // create a dummy message
                    auto message = MidiMessage::noteOn ((int)PlaybackSettings::get().getInstrument(),
                                                        0, (uint8) 0);
                    message.setTimeStamp (timeStamp);
                    MidiEventList::get().addMidiEvent (message);
                    
                    
                    // calculate the next value
                    timeStamp += PlaybackSettings::get().lengthOfNoteInMillisec (valueLeft);
                    
                    auto messageOff = MidiMessage::noteOff ((int)PlaybackSettings::get().getInstrument(),
                                                            0, (uint8) 0);
                    messageOff.setTimeStamp (timeStamp);
                    MidiEventList::get().addMidiEvent (messageOff);
                    
                    // update latest addition
                    PlaybackSettings::get().setLatestTimestampAddition (timeStamp);
                }
            }
        }
    };
    
    
    //==========================================================================
    
    /**
     *  A bar of 4/4 time containg a MusiSyncEng bar. 
     */
    class FourFourBar    : public BarBlock
    {
    public:
        /** Default constructor. */
        FourFourBar () : BarBlock ("FourFourBar",
                                   new MusiSyncEng::Bar (4,4),
                                   ImageCache::getFromMemory (BinaryData::fourFourBlockIcon_png,
                                                              BinaryData::fourFourBlockIcon_pngSize),
                                   ImageCache::getFromMemory (BinaryData::fourFourBlock_png,
                                                              BinaryData::fourFourBlock_pngSize),
                                   352)
        {
            // scale connections to fit the image used
            getScaling().setConnectionWidthScaling (0.045);
        }
        
        /** Calculates the timing for a bar. */
        void doAction() override
        {
            calculateBarTiming();
        }
    
    private:
    };
    
    //==========================================================================
    
    /**
     *  A bar of 3/4 time containg a MusiSyncEng bar.
     */
    class ThreeFourBar    : public BarBlock
    {
    public:
        /** Default constructor. */
        ThreeFourBar () : BarBlock ("ThreeFourBar",
                                    new MusiSyncEng::Bar (3,4),
                                    ImageCache::getFromMemory (BinaryData::threeFourBlockIcon_png,
                                                              BinaryData::threeFourBlockIcon_pngSize),
                                    ImageCache::getFromMemory (BinaryData::threeFourBlock_png,
                                                              BinaryData::threeFourBlock_pngSize),
                                    264)
        {
            // scale connections to fit the image used
            getScaling().setConnectionWidthScaling (0.060);
        }
        
        /** Calculates the timing for a bar. */
        void doAction() override
        {
            calculateBarTiming();
        }
        
    };
    
    //==========================================================================
    
    /**
     *  A bar of 2/4 time containg a MusiSyncEng bar.
     */
    class TwoFourBar    : public BarBlock
    {
    public:
        /** Default constructor. */
        TwoFourBar () : BarBlock ("TwoFourBar",
                                    new MusiSyncEng::Bar (2,4),
                                    ImageCache::getFromMemory (BinaryData::twoFourBlockIcon_png,
                                                               BinaryData::twoFourBlockIcon_pngSize),
                                    ImageCache::getFromMemory (BinaryData::twoFourBlock_png,
                                                               BinaryData::twoFourBlock_pngSize),
                                    180)
        {
            // scale connections to fit the image used
            getScaling().setConnectionWidthScaling (0.090); 
        }
        
        /** Calculates the timing for a bar. */
        void doAction() override
        {
            calculateBarTiming();
        }
        
    };
    
    //==========================================================================
    
    /**
     *  A bar of 5/4 time containg a MusiSyncEng bar.
     */
    class FiveFourBar    : public BarBlock
    {
    public:
        /** Default constructor. */
        FiveFourBar () : BarBlock ("FiveFourBar",
                                  new MusiSyncEng::Bar (5,4),
                                  ImageCache::getFromMemory (BinaryData::fiveFourBlockIcon_png,
                                                             BinaryData::fiveFourBlockIcon_pngSize),
                                  ImageCache::getFromMemory (BinaryData::fiveFourBlock_png,
                                                             BinaryData::fiveFourBlock_pngSize),
                                  440)
        {
            // scale connections to fit the image used
            getScaling().setConnectionWidthScaling (0.035);
        }
        
        /** Calculates the timing for a bar. */
        void doAction() override
        {
            calculateBarTiming();
        }
        
    };
    
} // namespace codetta
