/*
  ==============================================================================

    TempoChanger.h
    Created: 9 Dec 2018 9:15:10pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../audio/Audio.h"
#include "../libs/juckly/client/Block.h"
#include "BlockSlider.h"

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  A block that varies the tempo.
     */
    class TempoChanger    : public juckly::Block
    {
    public:
        /** Default constructor. */
        TempoChanger () : juckly::Block ( std::make_unique<juckly::CommandBlockSettings>
                                        ("TempoChanger",
                                         ImageCache::getFromMemory (BinaryData::tempoChangerIcon_png, BinaryData::tempoChangerIcon_pngSize),
                                         ImageCache::getFromMemory (BinaryData::orangeBlock_png, BinaryData::orangeBlock_pngSize),
                                         false /* not a start node */,
                                         new BlockSlider ("q+", -300.0, 300.0, 50.0, 0.8, -12),
                                         false /* dosen't take a parameter */
                                         )
                                        )
        {
        }
        
        /** Changes the tempo to the slider value. */
        void doAction() override
        {
            int currentBpm = PlaybackSettings::get().getBPM();
            
            // Increment the tempo to the slider value.
            auto increment = dynamic_cast<BlockSlider*>(getInternalUI());
            jassert (increment != nullptr); // Cast didn't work!
            if (increment != nullptr)
                PlaybackSettings::get().setBPM (increment->getValue() + currentBpm);
        }
        
    private:
    };
    
} // namespace codetta
