/*
 ==============================================================================
 
 Block.cpp
 Created: 18 Aug 2018 2:53:22pm
 Author:  Corey Ford
 
 ==============================================================================
 */

#include "Block.h"

/** Main namespace for the juckly library */
namespace juckly
{
    Block::Block (const String uniqueID,
                  Image toolboxIconRhs,
                  Image workspaceImageRhs,
                  BlockType blockType,
                  bool isStartNode,
                  Component* internalUIRhs,
                  bool takesParamRhs,
                  const int width,
                  const int height,
                  bool isGlobalRhs)
                    : gui (toolboxIconRhs, workspaceImageRhs, scaling),
                      internalUI (internalUIRhs),
                      blockID        (uniqueID),
                      takesParam (takesParamRhs),
                      isGlobal (isGlobalRhs)
    {
        initialise (blockType, isStartNode, width, height);
    }
    
    Block::Block (std::unique_ptr<BlockSettings> blockSettings)
                    : gui (blockSettings->getToolboxIcon(),
                           blockSettings->getWorkspaceImage(),
                           scaling),
                      internalUI (blockSettings->getInternalUI()),
                      blockID (blockSettings->getUniqueID()),
                      takesParam (blockSettings->getTakesParam()),
                      isGlobal (blockSettings->getIsGlobal())
    {
        initialise (blockSettings->getBlockType(),
                    blockSettings->getIsStartNode(),
                    blockSettings->getWidth(),
                    blockSettings->getHeight());
    }
    
    void Block::initialise (BlockType& blockType,
                            bool& isStartNode,
                            const int& width,
                            const int& height)
    {
        // set block scaling object to the input width & height
        scaling.setWidth (width);
        scaling.setHeight (height);
        
        // restrict block to default size
        setSize (scaling.getWidth(), scaling.getHeight());
        
        // get manipulator from factory.
        BlockManipulatorFactory factory;
        manipulator = factory.createManipulator (blockType, scaling);
        addAndMakeVisible (*manipulator);
        manipulator->setConnections (this);
        
        // A function block cannot take a parameter, as this is a parameter
        //  type itself!
        if (takesParam == true && blockType == BlockType::functionBlock)
            jassert (false);
        
        // You cannot add a component if also expecting a function block
        // parameter!
        if (takesParam == true && internalUI != nullptr)
            jassert (false);
        
        // setup bounds for an internal UI
        internalUIBounds = getLocalBounds().toFloat();
        internalUIBounds.removeFromLeft (scaling.getConnectionWidth()
                                         + scaling.getComponentBoarder());
        internalUIBounds.removeFromRight (scaling.getConnectionWidth()
                                          + scaling.getComponentBoarder());
        internalUIBounds.removeFromTop (scaling.getComponentBoarder());
        internalUIBounds.removeFromBottom (scaling.getComponentBoarder());
        
        // setup internal UI component
        if (internalUI != nullptr)
        {
            resized();
            repaint();
            
            addAndMakeVisible (internalUI);
        }
        
        // setup parameter if needed
        if (takesParam == true)
        {
            addAndMakeVisible (blockParam);
            resized();
            repaint();
        }
        
        // default listener to nullptr
        listener = nullptr;
        
        // setup connection information
        connection.setStartNode (isStartNode);
    }
    
    Block::~Block()
    {
        delete internalUI;
    }
    
    //==========================================================================
    // Manipulator Functions
    
    void Block::setConstrainer (ComponentBoundsConstrainer* constrainerRhs)
    {
        manipulator->setConstrainer(constrainerRhs);
    }
 
    void Block::mouseDown (const MouseEvent& e)
    {
        // set previous block's connection to null
        if(connection.getPreviousNode() != nullptr)
            connection.getPreviousNode()->getConnection().setNextNode (nullptr);
        
        // set this blocks connection to the previous block to null
        connection.setPreviousNode (nullptr);
        
        // manipulate this blocks movement
        manipulator->mouseDown (this, e);
        
        moveNeighbours();
    }
    
    void Block::mouseDrag (const MouseEvent& e)
    {
        listener->checkCollisions (this, true);
        
        manipulator->mouseDrag (this, e);
        
        moveNeighbours();
    }
    
    void Block::mouseUp (const MouseEvent& e)
    {
        listener->checkCollisions (this, false);
    }
    
    const std::unique_ptr<BlockManipulator>& Block::getManipulator() const
    {
        return manipulator;
    }
    
    void Block::setListener (Listener* newListener)
    {
        listener = newListener;
    }
    
    //==========================================================================
    //  Connection Functions
    
    Connection<Block>& Block::getConnection()
    {
        return connection;
    }
    
    void Block::moveNeighbours()
    {
        // capture this and the next blocks position
        auto current = connection.getNextNode();
        auto previous = this;
        
        // move neigbouring blocks by width
        while (current != nullptr)
        {
            //TODO: control structure logic
            // if previous block is a control structure
            {
                // set the top current position to after the previous connection width
            }
            //else do below...
            
            current->setTopLeftPosition( previous->getX()
                                        + ((previous->getScaling().getWidth()
                                            - previous->getScaling().getConnectionWidth()))
                                        ,  previous->getY());
            
            // move the input and outputs also
            current->getManipulator()->setConnections (current);
            
            // step to the next node in list
            previous = current;
            current = current->getConnection().getNextNode();
        }
    }
    
    //==========================================================================
    //  Gui Functions
    
    const Image& Block::getToolboxIcon() const
    {
        return gui.getToolboxIcon();
    }
    
    void Block::paint (Graphics& g)
    {
        gui.paint (g, getLocalBounds(),
                   internalUI != nullptr ? true : false,
                   internalUIBounds);
    }
    
    void Block::resized()
    {
        internalUIBounds = getLocalBounds().toFloat();
        internalUIBounds.removeFromLeft (scaling.getConnectionWidth()
                                         + scaling.getComponentBoarder());
        internalUIBounds.removeFromRight (scaling.getConnectionWidth()
                                          + scaling.getComponentBoarder());
        internalUIBounds.removeFromTop (scaling.getComponentBoarder());
        internalUIBounds.removeFromBottom (scaling.getComponentBoarder());
                
        if (internalUI != nullptr)
        {
            internalUI->setBounds (internalUIBounds.toNearestInt());
        }
        
        if (takesParam)
        {
            blockParam.setBounds (internalUIBounds.toNearestInt());
        }
    }
    
    BlockGUI& Block::getGui()
    {
        return gui;
    }
    
    BlockScaling& Block::getScaling()
    {
        return scaling;
    }
    
    //==========================================================================
    // Other

    const String& Block::getID() const
    {
        return blockID;
    }
    
    const bool Block::willTakeParam() const
    {
        return takesParam;
    }
    
    BlockParamHole& Block::getBlockParam()
    {
        return blockParam;
    }
    
    Component*& Block::getInternalUI()
    {
        return internalUI;
    }
    
    const bool& Block::isGlobalState() const
    {
        return isGlobal;
    }

} // namespace juckly 
