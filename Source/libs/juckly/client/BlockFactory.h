/*
  ==============================================================================

    BlockFactory.h
    Created: 19 Aug 2018 7:46:27pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../block/DemoBlocks.h"
#include "Block.h"

//==============================================================================
/** Main namespace for the juckly library */
namespace juckly
{
    /**
     *  Pure virual factory for creating blocks for the @see workspace.
     */
    class BlockFactory
    {
    public:
        /** Virtual Destructor*/
        virtual ~BlockFactory() {}
        
        /**
         *  Returns new blocks dragged onto the workspace.
         *  @param the unique ID for the block requested.
         *  @return a new block matching the input ID.
         */
        virtual Block* doMakeBlock (String& blockID) = 0;
    };
    
    //==========================================================================
    
    /**
     *  An example child factory creating all demo blocks. @see DemoBlocks.h/
     */
    class DemoBlockFactory    : public juckly::BlockFactory
    {
    public:
        /**
         *  Returns new blocks dragged onto the workspace.
         *  @param the unique ID for the block requested.
         *  @return a new block matching the input ID.
         */
        virtual juckly::Block* doMakeBlock (String& blockID) override
        {
            if (blockID == "DemoBlock")
                return new juckly::DemoBlock(new Slider (Slider::SliderStyle::Rotary,
                                                         Slider::TextEntryBoxPosition::NoTextBox));
            
            if (blockID == "DemoStartBlock") return new juckly::DemoStartBlock;
            
            if (blockID == "DemoFunctionBlock") return new juckly::DemoFunctionBlock(new TextEditor);
            
            if (blockID == "DemoFunctionBlockHolder") return new juckly::DemoFunctionBlockHolder;
            
            // If you've broken here the block you have tried to create dosen't exist.
            jassert(false);
            return nullptr;
        }
    };
    
} // namespace juckly
