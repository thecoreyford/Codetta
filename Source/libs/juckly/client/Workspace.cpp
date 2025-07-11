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
        binIsShrunk = false; // presume normal size
        
        // clear block arrays
        startBlocks.clear();
        blocks.clear();
        
        lastAddedBlock = nullptr;
        
        // Setup workspace size to match background 
        setSize (background.getWidth(), background.getHeight());
        
        #ifdef CODETTA
            addAndMakeVisible (codetta::Arcs::get());
        #endif
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
        
        #ifdef CODETTA
            codetta::Arcs::get().setBounds (getLocalBounds());
        #endif
        
        // Set bin bounds based on if shrunken.
        auto binBounds = visibleBounds;
        if (binIsShrunk)
        {
            binBounds.removeFromTop(visibleBounds.getHeight() - 64);
            binBounds.removeFromRight(visibleBounds.getWidth() - 52);
        }
        else
        {
            binBounds.removeFromTop(visibleBounds.getHeight() - 108);
            binBounds.removeFromRight(visibleBounds.getWidth() - 84);
        }
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
        jassert (block != nullptr); // a block object must have been returned!

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

        //----------------------------------------------------------------------
        // HOTFIX: block not connected when first added
        // Basically, allthough updated in the constructor of Block.cpp,
        // the left and right connection rectangles (bounds) are not updated
        // on creation here. Hence, we get the block...
        Block* componentToDrag = nullptr;
        if (block->getConnection().isStartNode()){
            componentToDrag = startBlocks.getLast();
            lastAddedBlock = startBlocks.getLast();
        }
        else{
            componentToDrag = blocks.getLast();
            lastAddedBlock = blocks.getLast();
        }
        // ... and set the connections based on itself!
        componentToDrag->getManipulator()->setConnections(componentToDrag);
        //----------------------------------------------------------------------
        
        #ifdef CODETTA
        codetta::UndoWidget::get().updateUndoStack();
        #endif
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
                jassert(block != NULL);
                jassert(otherBlock != NULL);
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
        if ((block->isLoadingIn() == false) && block->getBoundsInParent().intersects (bin.getBoundsInParent()))
        {
            const String type = block->getID(); //for data logging
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
                    codetta::ContextTracker::get().cleanLastUsed();
                    codetta::InfoBar::get().updateContents ("Deleted!");
                    codetta::UndoWidget::get().updateUndoStack();
                #endif
                LOG_STRING (type + " was deleted");
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
        
        // incase when loading the block manipulator isn't created
        if (blockMan.get() != nullptr && otherBlockMan.get() != nullptr)
        {
            // check input touching output
            if(blockMan->hasInput() && otherBlockMan->hasOutput())//look at this :(
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
            LOG_STRING ("Connection made between "
                        + lhsBlock->getID() + " and " + rhsBlock->getID());
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
    
    void Workspace::setBinIsShrunk (bool binShrunk)
    {
        binIsShrunk = binShrunk;
        resized();
    }
    
    //==========================================================================
    
    const OwnedArray<Block>& Workspace::getStartBlocks() const
    {
        return startBlocks;
    }

    const OwnedArray<Block>& Workspace::getOtherBlocks() const
    {
        return blocks;
    }

    //==========================================================================
    
    void Workspace::setVisibleBounds (Rectangle<int> visibleBoundsRhs)
    {
        visibleBounds = visibleBoundsRhs;
    }

    //==========================================================================

    void Workspace::clearWorkstation()
    {
        startBlocks.clearQuick(true);
        blocks.clearQuick(true);
    }

    void Workspace::mouseDown (const MouseEvent& e)
    {
        // Store where the co-ordinates where block should be pasted (for async)
        Clipboard::get().setPasteCoords(e.getMouseDownPosition());

        // if right clicked give the option to paste
        if (ModifierKeys::currentModifiers.isCtrlDown()
            || ModifierKeys::currentModifiers.isRightButtonDown())
        {
            PopupMenu menu;
            menu.addItem (1, "Paste");
            menu.showMenuAsync(PopupMenu::Options(), [&](int result){
                if (result == 1)
                {
                    auto& copiedItem = Clipboard::get().paste();
                    if (copiedItem != nullptr)
                    {
                        injectBlockFromDescription(copiedItem,
                                                   Clipboard::get().getPasteCoords());
                        LOG_STRING ("Block "
                                    + copiedItem->getStringAttribute("id")
                                    + " pasted");
                    }
                }
              
            });
        }
    }

    void Workspace::injectBlockFromDescription(std::unique_ptr<XmlElement>& item,
                                               const Point<int>& pasteCoords)
    {
        if (item != nullptr)
        {
            // turn paste XML into source details
            DragAndDropTarget::SourceDetails blockSource
                                            (item->getStringAttribute("id"),
                                             nullptr, pasteCoords);
            
            // add block
            itemDropped (blockSource);
                                   
            // load block specific stuff!
            getLastAddedBlock()->doSaveOrLoad (item.get(), juckly::Block::FileManipulator::load);
            
            //----------------------------------------------------------
            // do connections also ... (for each start block)...
            for (int i = 0 ; i < startBlocks.size(); ++i) {
                auto current = startBlocks[i];
                bool isNull = true;
                do {
                    isNull = true; // presume we are pointing to null
                    if (current != nullptr)
                    {
                        isNull = false;
                        checkCollisions (current, false);
                        current = current->getConnection().getNextNode();
                    }
                } while (!isNull);
            }
            //----------------------------------------------------------
        }
    }

} // namespace juckly
