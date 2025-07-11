/*
 ==============================================================================
 
 StartBlock.h
 Created: 24 Oct 2018 1:35:00pm
 Author:  Corey Ford
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../audio/Audio.h"
#include "../libs/juckly/client/Block.h"

//==============================================================================

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Creates a start block.
     */
    class StartBlock    : public juckly::Block
    {
    public:
        /** Default constructor - creates a start block, passing true as the start param. */
        StartBlock()
        : Block ("StartBlock",
                 ImageCache::getFromMemory (BinaryData::codettaStartBlock_png, BinaryData::codettaStartBlock_pngSize),
                 ImageCache::getFromMemory (BinaryData::codettaStartBlock_png, BinaryData::codettaStartBlock_pngSize),
                 juckly::BlockType::triggerBlock,
                 true,
                 nullptr,
                 false
                 ) { }
        
        /** First time initialisation of  all playback settings. */
        void doAction() override
        {
            PlaybackSettings::get().setLatestTimestampAddition (0);
            PlaybackSettings::get().clearAllStartRepeats();
            PlaybackSettings::get().registerStartRepeat (this);
            PlaybackSettings::get().setTuningOffset (0);
            PlaybackSettings::get().setBPM (PlaybackSettings::get().getGlobalTempo());
            doResets();
        }
        
        /** Resets block defaults */
        void doResets()
        {
            PlaybackSettings::get().setCurrentClef (MusiSyncEng::ClefTypes::Treble);
            PlaybackSettings::get().setInstrument (PlaybackSettings::Instrument::piano);
            PlaybackSettings::get().setVelocity (63);
        }
        
        //======================================================================
        
        /**
         *  Contains info for saving and loading an start block
         *  @param the head element for this block
         *  @param if save or load should be performed
         */
         void doSaveOrLoad (XmlElement* blockHead, FileManipulator mode) override{}
    };
    
} // namespace codetta

