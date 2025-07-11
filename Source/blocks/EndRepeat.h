/*
 ==============================================================================
 
 EndRepeat.h
 Created: 30 Oct 2018 10:01:13pm
 Author:  Corey Ford
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../audio/Audio.h"
#include "../libs/juckly/client/Block.h"
#include "BlockSlider.h"

using FragmentPtr = std::unique_ptr<MusiSyncEng::NoteFragment>;

//==============================================================================

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Class represening an end repeat bar.
     */
    class EndRepeat    : public juckly::Block
    {
    public:
        /** Default constructor. */
        EndRepeat () : juckly::Block ( std::make_unique<juckly::CommandBlockSettings>
                                      ("EndRepeat",
                                       ImageCache::getFromMemory (BinaryData::endRepeatBlockIcon_png, BinaryData::endRepeatBlockIcon_pngSize),
                                       ImageCache::getFromMemory (BinaryData::yellowBlock_png, BinaryData::yellowBlock_pngSize),
                                       false /* not a start node */,
                                       new BlockSlider ("}", 1.0, 10.0, 1.0, 1.3, -10, -10),
                                       false /* dosen't take a parameter */
                                       )
                                      )
        {
        }
        
        /** Logic for cycling from a start repeat block. */
        void doAction() override
        {
            auto slider = dynamic_cast<BlockSlider*>(getInternalUI());
            jassert (slider != nullptr); // Cast didn't work! :(
            if (slider != nullptr)
            {
                auto totalRepeats = slider->getValue();
                
                // for every specified repeat
                for (int i = 0; i < (int)totalRepeats; ++i)
                {
                    auto current = PlaybackSettings::get().getThisRepeat();
                    
                    if (current != nullptr)
                    {
                        // cycle through up to this repeat
                        while (current != this)
                        {
                            current->doAction();
                            
                            current = current->getConnection().getNextNode();
                        }
                        
                    }
                }
                
                
                PlaybackSettings::get().deregisterStartRepeat();
            }            
        }
        
    private:
    };
    
} // namespace codetta

