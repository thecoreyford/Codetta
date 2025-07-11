/*
 ==============================================================================
 
 Instruments.h
 Created: 11 Jan 2019 7:14:39pm
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
     *  Abstract class for all Instrument blocks.
     */
    class InstrumentBlock    : public juckly::Block
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param instrumentRhs is instrument corresponding to this block
         *  @param image is the icon for this block.
         *  @param reccomendedClef is text describing the best clef to use for the instrument
         */
        InstrumentBlock (String ID,
                         PlaybackSettings::Instrument instrumentRhs,
                         Image image,
                         const String reccomendedClef= "Treble")
        : Block (ID,
                 image,
                 image,
                 juckly::BlockType::triggerBlock,
                 true,
                 nullptr,
                 false
                 ), instrument (instrumentRhs)
        {
            InfoBar::get().updateContents ("Added " + ID + "!" + "     |     " +
                                           "Clef: " + reccomendedClef);
        }
        
        /** Virtual destructor */
        virtual ~InstrumentBlock() {}
        
         /** First time initialisation of  all playback settings. */
         void doAction() override
         {
             PlaybackSettings::get().setLatestTimestampAddition (0);
             PlaybackSettings::get().setCurrentClef (MusiSyncEng::ClefTypes::Treble);
             PlaybackSettings::get().setTuningOffset (0);
             PlaybackSettings::get().clearAllStartRepeats();
             PlaybackSettings::get().registerStartRepeat (this);
             PlaybackSettings::get().setBPM (PlaybackSettings::get().getGlobalTempo());
             PlaybackSettings::get().setVelocity (63);
             doResets();
         }
         
        /** Resets block defaults */
         void doResets()
         {
             PlaybackSettings::get().setInstrument (instrument);
//             PlaybackSettings::get().setVelocity (63);
//             PlaybackSettings::get().setTuningOffset (0);
         }
         
        
        //======================================================================
        
        /**
         *  Contains info for saving and loading an intstrument block
         *  @param the head element for this block
         *  @param if save or load should be performed
         */
         void doSaveOrLoad (XmlElement* blockHead, FileManipulator mode) override{}
        
    private:
        /** The instrument for this block.*/
        PlaybackSettings::Instrument instrument;
    };
    
    //==========================================================================
    /** Block setting subsequent notes to piano.*/
    class Piano   : public InstrumentBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         *  @param reccomendedClef is text describing the best clef to use for the instrument
         */
        Piano() : InstrumentBlock ("Piano",
                                   PlaybackSettings::Instrument::piano,
                                   ImageCache::getFromMemory (BinaryData::piano_png,
                                                              BinaryData::piano_pngSize),
                                   "Either"){ }
    };
    
    //==========================================================================
    /** Block setting subsequent notes to glockenspiel.*/
    class Glockenspiel   : public InstrumentBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         */
        Glockenspiel() : InstrumentBlock ("Glockenspiel",
                                          PlaybackSettings::Instrument::glockenspiel,
                                          ImageCache::getFromMemory (BinaryData::glockenspiel_png,
                                                                     BinaryData::glockenspiel_pngSize)){ }
    };
    
    //==========================================================================
    /** Block setting subsequent notes to acoustic guitar.*/
    class AcousticGuitar   : public InstrumentBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         */
        AcousticGuitar() : InstrumentBlock ("AcousticGuitar",
                                            PlaybackSettings::Instrument::acousticGuitar,
                                            ImageCache::getFromMemory (BinaryData::acousticGuitar_png,
                                                                       BinaryData::acousticGuitar_pngSize)){ }
    };
    
    //==========================================================================
    /** Block setting subsequent notes to distortion guitar.*/
    class DistortionGuitar   : public InstrumentBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         */
        DistortionGuitar() : InstrumentBlock ("DistortionGuitar",
                                              PlaybackSettings::Instrument::distortionGuitar,
                                              ImageCache::getFromMemory (BinaryData::distortionGuitar_png,
                                                                         BinaryData::distortionGuitar_pngSize)){ }
    };
    
    //==========================================================================
    /** Block setting subsequent notes to electric bass.*/
    class ElectricBass   : public InstrumentBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         *  @param reccomendedClef is text describing the best clef to use for the instrument
         */
        ElectricBass() : InstrumentBlock ("ElectricBass",
                                          PlaybackSettings::Instrument::electricBass,
                                          ImageCache::getFromMemory (BinaryData::electricBass_png,
                                                                     BinaryData::electricBass_pngSize),
                                          "Bass"){ }
    };
    
    //==========================================================================
    /** Block setting subsequent notes to violin.*/
    class Violin   : public InstrumentBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         */
        Violin() : InstrumentBlock ("Violin",
                                    PlaybackSettings::Instrument::violin,
                                    ImageCache::getFromMemory (BinaryData::violin_png,
                                                               BinaryData::violin_pngSize)){ }
    };
    
    //==========================================================================
    /** Block setting subsequent notes to viola.*/
    class Viola   : public InstrumentBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         */
        Viola() : InstrumentBlock ("Viola",
                                   PlaybackSettings::Instrument::viola,
                                   ImageCache::getFromMemory (BinaryData::viola_png,
                                                              BinaryData::viola_pngSize)){ }
    };
    
    //==========================================================================
    /** Block setting subsequent notes to cello.*/
    class Cello   : public InstrumentBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         */
        Cello() : InstrumentBlock ("Cello",
                                   PlaybackSettings::Instrument::cello,
                                   ImageCache::getFromMemory (BinaryData::cello_png,
                                                              BinaryData::cello_pngSize),
                                   "Bass"){ }
    };
    
    //==========================================================================
    /** Block setting subsequent notes to trumpet.*/
    class Trumpet   : public InstrumentBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         */
        Trumpet() : InstrumentBlock ("Trumpet",
                                     PlaybackSettings::Instrument::trumpet,
                                     ImageCache::getFromMemory (BinaryData::trumpet_png,
                                                                BinaryData::trumpet_pngSize)){ }
    };
    
    //==========================================================================
    /** Block setting subsequent notes to alto sax.*/
    class AltoSax   : public InstrumentBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         */
        AltoSax() : InstrumentBlock ("AltoSax",
                                     PlaybackSettings::Instrument::altoSax,
                                     ImageCache::getFromMemory (BinaryData::altoSax_png,
                                                                BinaryData::altoSax_pngSize)){ }
    };
    
    //==========================================================================
    /** Block setting subsequent notes to tenor sax.*/
    class TenorSax   : public InstrumentBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         */
        TenorSax() : InstrumentBlock ("TenorSax",
                                      PlaybackSettings::Instrument::tenorSax,
                                      ImageCache::getFromMemory (BinaryData::tenorSax_png,
                                                                 BinaryData::tenorSax_pngSize)){ }
    };
    
    //==========================================================================
    /** Block setting subsequent notes to bassoon.*/
    class Bassoon   : public InstrumentBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         */
        Bassoon() : InstrumentBlock ("Bassoon",
                                     PlaybackSettings::Instrument::basoon,
                                     ImageCache::getFromMemory (BinaryData::bassoon_png,
                                                                BinaryData::bassoon_pngSize)){ }
    };
    
    //==========================================================================
    /** Block setting subsequent notes to flute.*/
    class Flute   : public InstrumentBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         */
        Flute() : InstrumentBlock ("Flute",
                                   PlaybackSettings::Instrument::flute,
                                   ImageCache::getFromMemory (BinaryData::flute_png,
                                                              BinaryData::flute_pngSize)){ }
    };
    
    //==========================================================================
    /** Block setting subsequent notes to pad.*/
    class Pad   : public InstrumentBlock
    {
    public:
        /**
         *  Constructor.
         *  @param ID is the unique name for the block.
         *  @param clefRhs is the type of clef this refers to @see MusiSyncEng
         *  @param image is the icon for this block.
         *  @param reccomendedClef is text describing the best clef to use for the instrument
         */
        Pad() : InstrumentBlock ("Pad",
                                 PlaybackSettings::Instrument::pad,
                                 ImageCache::getFromMemory (BinaryData::pad_png,
                                                            BinaryData::pad_pngSize),
                                 "Either"){ }
    };
    
    
    
    
    
} // namespace codetta


