/*
  ==============================================================================

    TempoSetter.h
    Created: 30 Nov 2018 4:53:42pm
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
     *  A block that is a setter for the tempo.
     */
    class TempoSetter    : public juckly::Block
    {
    public:
        /** Default constructor. */
        TempoSetter () : juckly::Block ( std::make_unique<juckly::CommandBlockSettings>
                                        ("TempoSetter",
                                         ImageCache::getFromMemory (BinaryData::tempoSetterIcon_png, BinaryData::tempoSetterIcon_pngSize),
                                         ImageCache::getFromMemory (BinaryData::honeydewBlock_png, BinaryData::honeydewBlock_pngSize),
                                         false /* not a start node */,
                                         new BlockSlider ("q=", 24.0, 550.0, 120.0, 0.8, -12),
                                         false /* dosen't take a parameter */
                                         )
                                        )
        {
        }
        
        /** Changes the tempo to the slider value. */
        void doAction() override
        {
            // Change the tempo to the slider value.
            auto tempo = dynamic_cast<BlockSlider*>(getInternalUI());
            jassert (tempo != nullptr); // Cast didn't work!
            if (tempo != nullptr)
                PlaybackSettings::get().setBPM (tempo->getValue());
        }
        
    private:
    };
    
} // namespace codetta
