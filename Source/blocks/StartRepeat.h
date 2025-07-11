/*
  ==============================================================================

    StartRepeat.h
    Created: 29 Oct 2018 10:48:15am
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../audio/Audio.h"
#include "../libs/juckly/client/Block.h"

using FragmentPtr = std::unique_ptr<MusiSyncEng::NoteFragment>;

//==============================================================================

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  A start repeat block.
     */
    class StartRepeat    : public juckly::Block
    {
    public:
        /** Default constructor. */
        StartRepeat () : juckly::Block ( std::make_unique<juckly::CommandBlockSettings>
                                        ("StartRepeat",
                                         ImageCache::getFromMemory (BinaryData::startRepeatBlock_png, BinaryData::startRepeatBlock_pngSize),
                                         ImageCache::getFromMemory (BinaryData::startRepeatBlock_png, BinaryData::startRepeatBlock_pngSize),
                                         false /* not a start node */,
                                         nullptr,
                                         false /* dosen't take a parameter */,
                                         70
                                         )
                                        )
        {
            // scale connections to fit the image used
            getScaling().setConnectionWidthScaling (0.23);
        }
        
        /** Registers this bar as a start repeat. */
        void doAction() override
        {
            PlaybackSettings::get().registerStartRepeat (this);
        }
        
    private:
    };
    
} // namespace codetta
