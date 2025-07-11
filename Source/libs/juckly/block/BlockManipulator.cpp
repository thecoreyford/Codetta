/*
 ==============================================================================
 
 BlockManipulator.cpp
 Created: 28 Aug 2018 1:23:30pm
 Author:  Corey Ford
 
 ==============================================================================
 */

#include "BlockManipulator.h"

/** Main namespace for the juckly library */
namespace juckly
{
    BlockManipulator::BlockManipulator (BlockScaling& scalingRhs) : scaling (scalingRhs)
    {
        
    }

    void BlockManipulator::setConstrainer (ComponentBoundsConstrainer* constrainerRhs)
    {
        constrainer = constrainerRhs;
    }
    
    Rectangle<float> BlockManipulator::getInput()
    {
        jassert(false);

		Rectangle<float> dummy; // for windows
		return dummy;
    }
    
    Rectangle<float> BlockManipulator::getOutput()
    {
        jassert(false);

		Rectangle<float> dummy; // for windows
		return dummy;
    }
    
    Rectangle<float> BlockManipulator::getParam()
    {
        jassert(false);

		Rectangle<float> dummy; // for windows
		return dummy;
    }
    //==========================================================================
    
    CommandBlockManipulator::CommandBlockManipulator (BlockScaling& scaling) : BlockManipulator (scaling)
    {
        // set default values from a leftmost point index
        input.setSize (scaling.getConnectionWidth(),
                       scaling.getHeight());
        
        output.setSize (scaling.getConnectionWidth(),
                        scaling.getHeight());
        output.translate (scaling.getWidth() - scaling.getConnectionWidth(), 0);
    }
    
    void CommandBlockManipulator::mouseDown (Component* block,  const MouseEvent& e)
    {
        dragger.startDraggingComponent (block, e);
    }
    
    void CommandBlockManipulator::mouseDrag (Component* block, const MouseEvent& e)
    {
        dragger.dragComponent (block, e, constrainer);
        
        setConnections (block);
    }
    
    const bool CommandBlockManipulator::hasInput() { return true; }
    
    Rectangle<float> CommandBlockManipulator::getInput() { return input; }
    
    const bool CommandBlockManipulator::hasOutput() { return true; }
    
    Rectangle<float> CommandBlockManipulator::getOutput() { return output; }
    
    const bool CommandBlockManipulator::isParam() { return false; }
    
    void CommandBlockManipulator::setConnections (Component* block)
    {
        input.setX (block->getX());
        input.setY (block->getY());
        
        output.setX (block->getX());
        output.setY (block->getY());
        output.translate (scaling.getWidth() - scaling.getConnectionWidth(), 0);
    }
    
    //==========================================================================
    
    TriggerBlockManipulator::TriggerBlockManipulator (BlockScaling& scaling) : BlockManipulator (scaling)
    {
        output.setSize (scaling.getConnectionWidth(),
                        scaling.getHeight());
        output.translate (scaling.getWidth() - scaling.getConnectionWidth(), 0);
    }
    
    void TriggerBlockManipulator::mouseDown (Component* block, const MouseEvent& e)
    {
        dragger.startDraggingComponent (block, e);
        
        setConnections (block);
    }
    
    void TriggerBlockManipulator::mouseDrag (Component* block, const MouseEvent& e)
    {
        dragger.dragComponent (block, e, constrainer);
    
        setConnections (block);
    }
    
    const bool TriggerBlockManipulator::hasInput() { return false; }
        
    const bool TriggerBlockManipulator::hasOutput() { return true; }
    
    Rectangle<float> TriggerBlockManipulator::getOutput() { return output; }
    
    const bool TriggerBlockManipulator::isParam() { return false; }
    
    void TriggerBlockManipulator::setConnections (Component* block)
    {
        output.setX (block->getX());
        output.setY (block->getY());
        output.translate (scaling.getWidth() - scaling.getConnectionWidth(), 0);
    }
    
    //==========================================================================
    FunctionBlockManipulator::FunctionBlockManipulator (BlockScaling& scaling) : BlockManipulator (scaling)
    {
        // setup bounds for parameter size
        paramBounds.setSize (scaling.getWidth(), scaling.getHeight());
        paramBounds.removeFromLeft (scaling.getConnectionWidth()
                                         + scaling.getComponentBoarder());
        paramBounds.removeFromRight (scaling.getConnectionWidth()
                                          + scaling.getComponentBoarder());
        paramBounds.removeFromTop (scaling.getComponentBoarder());
        paramBounds.removeFromBottom (scaling.getComponentBoarder());
    }
    
    void FunctionBlockManipulator::mouseDown (Component* block, const MouseEvent& e)
    {
        dragger.startDraggingComponent (block, e);
    }
    
    void FunctionBlockManipulator::mouseDrag (Component* block, const MouseEvent& e)
    {
        dragger.dragComponent (block, e, constrainer);
        
        setConnections (block);
    }
    
    const bool FunctionBlockManipulator::hasInput() { return false; }
    
    const bool FunctionBlockManipulator::hasOutput() { return false; }
    
    const bool FunctionBlockManipulator::isParam() { return true; }
    
    Rectangle<float> FunctionBlockManipulator::getParam() { return paramBounds; }
    
    void FunctionBlockManipulator::setConnections (Component* block)
    {
        paramBounds.setX (block->getX());
        paramBounds.setY (block->getY());
    }
    
} // namespace juckly
