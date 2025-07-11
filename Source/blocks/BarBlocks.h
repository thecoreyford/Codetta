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
#include "../gui/LiveGuiPlayback.h"
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
                                          ){ }
        
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
                        
                        // caputure the message note (in light of pitch offsets)
                        auto messageNote = PlaybackSettings::get().getTuningOffsetCMajorMIDIValue (fragment[j]->getMusicalValue());
                        
                        //====================================================================================
                        // translate into message on
                        auto messageOn = MidiMessage::noteOn ((int)PlaybackSettings::get().getInstrument(),
                                                              messageNote,
                                                              (uint8) PlaybackSettings::get().getVelocity());
                        messageOn.setTimeStamp (timeStamp); //< add timestamp
                        MidiEventList::get().addMidiEvent (messageOn); //< add to event list
                        LiveGuiPlayback::get().add (&(*(fragment[j])), true, timeStamp); //< update gui feeback
                        
                        //====================================================================================
                        
                        // calculate the next value
                        timeStamp += PlaybackSettings::get().lengthOfNoteInMillisec (fragment[j]->getValue());
                        
                        //timeStamp += PlaybackSettings::get().getLatestTimestampAddition();
                        
                        // add note off
                        auto messageOff = MidiMessage::noteOff ((int)PlaybackSettings::get().getInstrument(),
                                                                messageNote,
                                                                (uint8) PlaybackSettings::get().getVelocity());
                        messageOff.setTimeStamp (timeStamp); //< add timestamp
                        MidiEventList::get().addMidiEvent (messageOff); // add to event list
                        LiveGuiPlayback::get().add (&(*(fragment[j])), false, timeStamp); //< update gui feeback
                        
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
                    LiveGuiPlayback::get().add (nullptr, true, timeStamp); //< update gui feeback
                    
                    
                    // calculate the next value
                    timeStamp += PlaybackSettings::get().lengthOfNoteInMillisec (valueLeft);
                    
                    auto messageOff = MidiMessage::noteOff ((int)PlaybackSettings::get().getInstrument(),
                                                            0, (uint8) 0);
                    messageOff.setTimeStamp (timeStamp);
                    MidiEventList::get().addMidiEvent (messageOff);
                    LiveGuiPlayback::get().add (nullptr, false, timeStamp); //< update gui feeback

                    
                    // update latest addition
                    PlaybackSettings::get().setLatestTimestampAddition (timeStamp);
                }
            }
        }

        
        //======================================================================
        
        /**
         *  Contains info for saving and loading a bar block
         *  @param the head element for this block
         *  @param if save or load should be performed
         */
        void forBarSaveOrLoad (XmlElement* blockHead, juckly::Block::FileManipulator mode)
        {
            auto bar = dynamic_cast<MusiSyncEng::Bar*>(getInternalUI());
            
            if (mode == juckly::Block::FileManipulator::save)
            {
                auto& frag = bar->getFragment();
                for (auto& f : frag)
                {
                    // Should only ever have one note in the fragment
                    // for codetta - allthough flexibility is there!
                    jassert (f->getNoOfNotesInFragment() == 1);
                    
                    auto n = f->getFirstNoteOfFragment();
                    
                    XmlElement* note = new XmlElement ("Note");
                    note->setAttribute ("length", (*n)->getValue());
                    note->setAttribute ("pitchOffset", (*n)->getPitchOffset());
                    blockHead->addChildElement (note);
                }
            }
            
            //==================================================================
            
            if (mode == juckly::Block::FileManipulator::load)
            {
                 forEachXmlChildElement (*blockHead, e)
                 {
                     if(e->hasTagName("Note"))
                     {
                         MusiSyncEng::Fragment length = (MusiSyncEng::Fragment)e->getIntAttribute ("length");
                         int pitchOffset = e->getIntAttribute ("pitchOffset");
                         
                         // create the note.
                         bar->onFragmentSelected (length);
                         
                         auto& frag = bar->getFragment();
                         auto& note = frag.back();
                         auto n = note->getFirstNoteOfFragment();
                         (*n)->setPitchOffset (pitchOffset);
                         
                         // Should only ever have one note in the fragment
                         // for codetta - allthough flexibility is there!
                         jassert (note->getNoOfNotesInFragment() == 1);
                     }
                 }
                
                
                
                //bar->onFragmentSelected(
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
                                   new MusiSyncEng::Bar (4,4, this),
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
                
        /**
         *  Contains info for saving and loading a four four bar
         *  @param the head element for this block
         *  @param if save or load should be performed
         */
        void doSaveOrLoad (XmlElement* blockHead, juckly::Block::FileManipulator mode) override
        {
            forBarSaveOrLoad (blockHead, mode);
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
                                    new MusiSyncEng::Bar (3,4, this),
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
        
        /**
         *  Contains info for saving and loading a three four bar
         *  @param the head element for this block
         *  @param if save or load should be performed
         */
        void doSaveOrLoad (XmlElement* blockHead, juckly::Block::FileManipulator mode) override
        {
            forBarSaveOrLoad (blockHead, mode);
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
                                    new MusiSyncEng::Bar (2,4, this),
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
        
        /**
         *  Contains info for saving and loading a two four bar
         *  @param the head element for this block
         *  @param if save or load should be performed
         */
        void doSaveOrLoad (XmlElement* blockHead, juckly::Block::FileManipulator mode) override
        {
            forBarSaveOrLoad (blockHead, mode);
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
                                  new MusiSyncEng::Bar (5,4, this),
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
        
        /**
         *  Contains info for saving and loading a five four bar
         *  @param the head element for this block
         *  @param if save or load should be performed
         */
        void doSaveOrLoad (XmlElement* blockHead, juckly::Block::FileManipulator mode) override
        {
            forBarSaveOrLoad (blockHead, mode);
        }
        
    };
    
} // namespace codetta
