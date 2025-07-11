/*
  ==============================================================================

    Workspace.h
    Created: 14 Aug 2018 11:59:24am
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../block/BlockScaling.h"
#include "BlockFactory.h"
#include "Block.h"

//==============================================================================

#define CODETTA 0 //< add macro so that the libs folder can remain clean for
                  // potentially releasing as an API.
#ifdef CODETTA
    #include "../../../gui/InfoBar.h"
#endif

//==============================================================================

/** Main namespace for the juckly library */
namespace juckly
{
    /**
     *  The main container for holding every created block.
     */
    class Workspace    : public Component,
                         public DragAndDropTarget,
                         public Block::Listener
    {
    public:
        /**
         *  Constructor. Intitialises factory pointer to null.
         */
        Workspace();
        
        /**
         *  Destructor. Deletes factory pointer.
         */
        ~Workspace();
        
        /**
         *  Setter for incoporating certain block factory's. @see BlockFactory.h
         *  @param the pointer to the factory to create blocks.
         */
        void injectBlockFactory (std::shared_ptr<BlockFactory> factory);
        
        /**
         *  Draws the background for the workspace and non-component visuals. 
         *  @param the graphics context.
         */
        void paint (Graphics&) override;
        
        /**
         *  Sets bounds of child components & ensures constrainer is correct to 
         *  screen size.
         */
        void resized() override;
        
        /**
         *  Callback from initial drag. We can only drag blocks so true is returned. 
         *  @param dragSourceDetails contains information about what's been dragged.
         *  @return if we want to recive callbacks from whats added.
         */
        bool isInterestedInDragSource (const SourceDetails& dragSourceDetails) override;
    
        /**
         *  Callback for when an item has been dropped onto the workspace. 
         *  @param dragSourceDetails contains information about what's been dragged.
         */
        void itemDropped (const SourceDetails& dragSourceDetails) override;
        
        /**
         *  Callback broadcast from @see block to check collisions against other
         *  blocks in workspace and that being dragged.
         *  @param the block being dragged.
         *  @param true if called from mouseDrag callback.
         */
        void checkCollisions (Block* block, bool isMouseDrag) override;
        
        /**
         *  If blocks inputs/outputs match this creates connections
         *  accordingly.
         *  @param the block on the left hand side of the gui.
         *  @param the block on the right hand side of the gui.
         */
        void ifPossibleSnapBlocks (Block* lhsBlock, Block* rhsBlock);
        
        /**
         *  Getter for all starting blocks. 
         *  Useful for writing custom iterators for chains of blocks.
         *  @return reference to the array of all starting blocks.
         */
        const OwnedArray<Block>& getStartBlocks() const;
        
        /**
         *  Setter for the visible bounds.
         *  @param the new visible bounds area.
         */
        void setVisibleBounds (Rectangle<int> visibleBoundsRhs);

    private:
        
        /**
         *  Checks if inputs and outputs for blocks match.
         *  @param the block being dragged.
         *  @param the block checking for connections against.
         */
        void checkAllConnections (Block* block, Block* otherBlock);
        
        /**
         *  Check connections and toggles state to hilight block connections.
         *  @param the block being dragged.
         *  @param the block checking for connections against.
         */
        void hilightConnectionIfNear (Block* block, Block* otherBlock);
    
        /** Constrainer. Ensures created blocks stay within the workspace.*/
        ComponentBoundsConstrainer constrainer;
        
        /** Array of newly added regular blocks to the workspace. */
        OwnedArray<Block> blocks;
        
        /** Array tracking all start node blocks. */
        OwnedArray<Block> startBlocks;
        
        /** Pointer to the block factory. */
        std::shared_ptr<BlockFactory> blockFactory;
        
        /** Image representing the bin. */
        ImageComponent bin;
        
        /** Image representing the workspace background. */
        Image background {ImageCache::getFromMemory (BinaryData::loopingdesk_jpg,
                                                       BinaryData::loopingdesk_jpgSize)};
        
        /** The rectangle visible, incase workspace is housed within a viewport */
        Rectangle<int> visibleBounds;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Workspace)
    };
    
} // namespace juckly
