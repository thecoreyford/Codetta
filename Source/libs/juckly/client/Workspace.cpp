/*
 ==============================================================================
 
 Workspace.cpp
 Created: 14 Aug 2018 11:59:24am
 Author:  Corey Ford
 
 ==============================================================================
 */

#include "Workspace.h"

//==============================================================================

/** Main namespace for the juckly library */
namespace juckly
{
    Workspace::Workspace()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        blockFactory = nullptr;
        
        // Add bin component
        bin.setImage (ImageCache::getFromMemory(BinaryData::binClosed_png,
                                        BinaryData::binClosed_pngSize));
        addAndMakeVisible (bin);
        
        // clear block arrays
        startBlocks.clear();
        blocks.clear();
        
        // Setup workspace size to match background 
        setSize (background.getWidth(), background.getHeight());
    }
    
    Workspace::~Workspace()
    {
    }
    
    void Workspace::injectBlockFactory (std::shared_ptr<BlockFactory> factory)
    {
        blockFactory = factory;
    }
    
    void Workspace::paint (Graphics& g)
    {
        g.setTiledImageFill(background, 0, 0, 1.0f);
        g.fillAll();
    }
    
    void Workspace::resized()
    {
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
                                               getHeight(), getWidth());
        
        auto binBounds = visibleBounds;
        binBounds.removeFromTop(visibleBounds.getHeight() - 108);
        binBounds.removeFromRight(visibleBounds.getWidth() - 84);
        bin.setBounds (binBounds);
    }
    
    bool Workspace::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
    {
        return true;
    }
    
    void Workspace::itemDropped (const SourceDetails& dragSourceDetails)
    {
        String description = dragSourceDetails.description;
        
        // Make sure you call injectBlockFactory after instatiating the workspace.
        jassert(blockFactory != nullptr);
        
        // Use factory to create blocks matching description
        auto block = blockFactory->doMakeBlock (description);
        
        // Test for global blocks first before doing anything else!
        if (block->isGlobalState())
        {
            // check to see that it hasn't already been added
            bool inArray = false;
            for (auto b : startBlocks)
            {
                if (b->getID() == description)
                {
                    inArray = true;
                }
            }
            
            // if it hasn't already been added
            if (inArray == false)
            {
                // place it at the front of our playback
                startBlocks.insert (0, block);
                startBlocks[0]->setConstrainer (&constrainer);
                startBlocks[0]->setBounds (0, 0, block->getScaling().getWidth(),
                                           block->getScaling().getHeight());
                startBlocks[0]->setCentrePosition (dragSourceDetails.localPosition);
                startBlocks[0]->setListener (this);
                addAndMakeVisible (startBlocks[0]);
            }
        }
        else
        {
            if (block->getConnection().isStartNode())
            {
                startBlocks.add (block);
                startBlocks.getLast()->setConstrainer (&constrainer);
                startBlocks.getLast()->setBounds (0, 0, block->getScaling().getWidth(),
                                                  block->getScaling().getHeight());
                startBlocks.getLast()->setCentrePosition (dragSourceDetails.localPosition);
                startBlocks.getLast()->setListener (this);
                addAndMakeVisible (startBlocks.getLast());
            }
            else
            {
                blocks.add (block);
                blocks.getLast()->setConstrainer (&constrainer);
                blocks.getLast()->setBounds (0, 0, block->getScaling().getWidth(),
                                             block->getScaling().getHeight());
                blocks.getLast()->setCentrePosition (dragSourceDetails.localPosition);
                blocks.getLast()->setListener (this);
                addAndMakeVisible (blocks.getLast());
            }
        }
    }
    
    void Workspace::checkCollisions (Block* block, bool isMouseDrag)
    {
        // Compare the moving block to the other blocks within the workspace.
        for (auto otherBlock : blocks)
        {
            if (isMouseDrag)
            {
                hilightConnectionIfNear (block, otherBlock);
            }
            else
            {
                checkAllConnections (block, otherBlock);
            }
        }
        
        // Also compare with the start node blocks.
        for (auto otherBlock : startBlocks)
        {
            if (isMouseDrag)
            {
                hilightConnectionIfNear (block, otherBlock);
            }
            else
            {
                checkAllConnections (block, otherBlock);
            }
        }
        
        // Compare the block with the bin component
        if (block->getBoundsInParent().intersects (bin.getBoundsInParent()))
        {
            if (isMouseDrag)
            {
                bin.setImage (ImageCache::getFromMemory (BinaryData::binOpen_png,
                                                         BinaryData::binOpen_pngSize));
            }
            else
            {
                auto currentBlock = block;
                do
                {
                    if (currentBlock->getConnection().isStartNode())
                    {
                        auto temp = currentBlock->getConnection().getNextNode();
                        startBlocks.removeObject (currentBlock);
                        currentBlock = temp;
                        
                    }
                    else if (!currentBlock->getConnection().isStartNode())
                    {
                        auto temp = currentBlock->getConnection().getNextNode();
                        blocks.removeObject (currentBlock);
                        currentBlock = temp;
                    }
                } while (currentBlock != nullptr);
                #ifdef CODETTA
                    codetta::InfoBar::get().updateContents ("Deleted!");
                #endif
            }
        }
        
        // reset bin image
        if (!isMouseDrag)
        {
            bin.setImage (ImageCache::getFromMemory (BinaryData::binClosed_png,
                                                     BinaryData::binClosed_pngSize));
        }
    }
    
    
    void Workspace::checkAllConnections (Block* block, Block* otherBlock)
    {
        auto& blockMan = block->getManipulator();
        auto& otherBlockMan = otherBlock->getManipulator();
        
        // check input touching output
        if(blockMan->hasInput() && otherBlockMan->hasOutput())
        {
            ifPossibleSnapBlocks(otherBlock, block);
        }
        
        // check output touching input
        if(blockMan->hasOutput() && otherBlockMan->hasInput())
        {
            ifPossibleSnapBlocks(block, otherBlock);
        }
        
        // Check parameter blocks touching
        if (blockMan->isParam() && otherBlock->willTakeParam())
        {
            auto movingBounds = block->getManipulator()->getParam();
            auto otherBounds = otherBlock->getBounds().toFloat();
            if (movingBounds.intersects (otherBounds))
            {
                block->setVisible(false);
                otherBlock->getBlockParam().addInternalParam (block);
            }
        }
    }
    
    void Workspace::ifPossibleSnapBlocks (Block* lhsBlock, Block* rhsBlock)
    {
        auto& blockMan = rhsBlock->getManipulator();
        auto& otherBlockMan = lhsBlock->getManipulator();
        
        auto blockRect = blockMan->getInput();
        auto otherBlockRect = otherBlockMan->getOutput();
        
        if (blockRect.intersects (otherBlockRect))
        {
            // Remove any highlighting.
            rhsBlock->getGui().setOverlapState (lhsBlock, OverlapState::noOverlap);
            lhsBlock->getGui().setOverlapState (lhsBlock, OverlapState::noOverlap);
            
            // Start node blocks cannot have inputs.
            // Check that you're block type in your blocks constructor parameter list is
            // a trigger block - or change its startNode status!
            jassert (rhsBlock->getConnection().isStartNode() == false);
            
            // Reset blocks previous connections
            if(rhsBlock->getConnection().getPreviousNode() != nullptr)
                rhsBlock->getConnection().getPreviousNode()->getConnection().setNextNode (nullptr);
            if(lhsBlock->getConnection().getNextNode() != nullptr)
            {
                auto connectedNode = lhsBlock->getConnection().getNextNode();
                
                // Move the block out of the way of the chain now its disconnected.
                connectedNode->setTopLeftPosition(connectedNode->getX(),
                                                  connectedNode->getY()
                                                  + connectedNode->getScaling().getHeight());
                connectedNode->moveNeighbours();
                
                // Set to null
                connectedNode->getConnection().setNextNode (nullptr);
            }
            
            // The right blocks input should be the left block
            rhsBlock->getConnection().setPreviousNode (lhsBlock);
            
            // The left blocks input output should be the right block
            lhsBlock->getConnection().setNextNode (rhsBlock);
            
            // Position blocks for a more aligned snap
            lhsBlock->moveNeighbours();
            
            // Add to info bar (codetta)
            #ifdef CODETTA
                codetta::InfoBar::get().updateContents ("Connection made between "
                                                        + lhsBlock->getID() + " and "
                                                        + rhsBlock->getID() + "!");
            #endif
        }
    }
    
    void Workspace::hilightConnectionIfNear (Block* block, Block* otherBlock)
    {
        if(otherBlock->getManipulator()->hasInput() && block->getManipulator()->hasOutput())
        {
            //if moving block output overlaps other block input
            auto movingOut = block->getManipulator()->getOutput();
            auto otherInput = otherBlock->getManipulator()->getInput();
            if (movingOut.intersects (otherInput))
            {
                otherBlock->getGui().setOverlapState(otherBlock, OverlapState::inputOverlap);
            }
            else
            {
                otherBlock->getGui().setOverlapState(otherBlock, OverlapState::noOverlap);
            }
        }
        
    
        if(block->getManipulator()->hasInput() && otherBlock->getManipulator()->hasOutput())
        {
            //if moving block input overlaps other block output
            auto movingInput = block->getManipulator()->getInput();
            auto otherOutput = otherBlock->getManipulator()->getOutput();
            if (movingInput.intersects (otherOutput))
            {
                otherBlock->getGui().setOverlapState(otherBlock, OverlapState::outputOverlap);
            }
            else
            {
                otherBlock->getGui().setOverlapState(otherBlock, OverlapState::noOverlap);
            }
        }
        
        // Check parameter blocks
        if (block->getManipulator()->isParam() && otherBlock->willTakeParam())
        {
            auto movingBounds = block->getManipulator()->getParam();
            auto otherBounds = otherBlock->getBounds().toFloat();
            if (movingBounds.intersects (otherBounds))
            {
                otherBlock->getGui().setOverlapState(otherBlock, OverlapState::paramOverlap);
            }
            else
            {
                otherBlock->getGui().setOverlapState(otherBlock, OverlapState::noOverlap);
            }
        }
    }
    //==========================================================================
    
    const OwnedArray<Block>& Workspace::getStartBlocks() const
    {
        return startBlocks;
    }

    //==========================================================================
    
    void Workspace::setVisibleBounds (Rectangle<int> visibleBoundsRhs)
    {
        visibleBounds = visibleBoundsRhs;
    }

} // namespace juckly
