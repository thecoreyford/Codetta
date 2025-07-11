/*
  ==============================================================================

    DynamicsChanger.h
    Created: 1 Feb 2019 3:41:58pm
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
    class DynamicsChanger    : public juckly::Block
    {
    public:
        /** Default constructor. */
        DynamicsChanger () : juckly::Block ( std::make_unique<juckly::CommandBlockSettings>
                                         ("DynamicsChanger",
                                          ImageCache::getFromMemory (BinaryData::dynamicsChangerIcon_png, BinaryData::dynamicsChangerIcon_pngSize),
                                          ImageCache::getFromMemory (BinaryData::darkPinkBlock_png, BinaryData::darkPinkBlock_pngSize),
                                          false /* not a start node */,
                                          new BlockSlider ("Dyn +", -64.0, 64.0, 10.0, 0.0, 0.0, 0.0, false),
                                          false /* dosen't take a parameter */
                                          )
                                         )
        {
        }
        
        /** Changes the tempo to the slider value. */
        void doAction() override
        {
            int currentVel = PlaybackSettings::get().getVelocity();
            
            // Increment the tempo to the slider value.
            auto increment = dynamic_cast<BlockSlider*>(getInternalUI());
            jassert (increment != nullptr); // Cast didn't work!
            if (increment != nullptr)
                PlaybackSettings::get().setVelocity (increment->getValue() + currentVel);
        }
        
    private:
    };
    
} // namespace codetta
