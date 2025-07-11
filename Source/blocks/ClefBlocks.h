/*
  ==============================================================================

    ClefBlocks.h
    Created: 28 Nov 2018 3:07:04pm
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
     *  Abstract class for all clef blocks.
     */
    class ClefBlock    : public juckly::Block
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         */
        ClefBlock (String ID,
                   MusiSyncEng::ClefTypes clefRhs,
                   Image image)
                        : juckly::Block (std::make_unique<juckly::CommandBlockSettings>
                                            (ID,
                                             image,
                                             image,
                                             false,
                                             nullptr,
                                             false
                                             )
                                        ), clef (clefRhs) {}
        
        /**
         *  Run when reading block, sets current clef.
         */
        void doAction() override
        {
            PlaybackSettings::get().setCurrentClef (clef);
        }
        
        //======================================================================
        
        /**
         *  Contains info for saving and loading a clef
         *  @param the head element for this block
         *  @param if save or load should be performed
         */
         void doSaveOrLoad (XmlElement* blockHead, FileManipulator mode) override{}
        
    private:
        /** The clef for this block.*/
        MusiSyncEng::ClefTypes clef;
    };
    
    //==========================================================================
    /** Block setting subsequent notes to treble clef.*/
    class TrebleClef    : public ClefBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         */
        TrebleClef() : ClefBlock ("TrebleClef",
                                  MusiSyncEng::ClefTypes::Treble,
                                  ImageCache::getFromMemory (BinaryData::trebleClefBlock_png,
                                                             BinaryData::trebleClefBlock_pngSize)
                                  )
        {
            
        }
    };
    
    //==========================================================================
    /** Block setting subsequent notes to bass clef.*/
    class BassClef    : public ClefBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         */
        BassClef() : ClefBlock ("BassClef",
                                MusiSyncEng::ClefTypes::Bass,
                                ImageCache::getFromMemory (BinaryData::bassClefBlock_png,
                                                           BinaryData::bassClefBlock_pngSize)
                                )
        {
            
        }
    };
    
} // namespace codetta
