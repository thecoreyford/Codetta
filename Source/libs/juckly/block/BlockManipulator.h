/*
  ==============================================================================

    BlockManipulator.h
    Created: 27 Aug 2018 4:03:03pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "BlockScaling.h"

//==============================================================================

/** Main namespace for the juckly library */
namespace juckly
{
    /**
     *  Pure virtual class for handling the moving of block components.
     */
    class BlockManipulator    :  public Component
    {
    public:
        /**
         *  Constructor.
         *  @param scaling is the sizing information for the block object.
         */
        BlockManipulator (BlockScaling& scalingRhs);
        
        /** Virtual Destructor. */
        virtual ~BlockManipulator() { }
        
        /**
         *  Setter for the component bounds constainer. 
         *  @param the desired component bounds constrainer.
         */
        void setConstrainer (ComponentBoundsConstrainer* constrainerRhs);
        
        /**
         *  Mouse down callback. Can call dragger's startDraggingComponent to
         *  move the block passed.
         *  @param block is a pointer to the object to be manipulated.
         *  @param e is the mouse down event.
         */
        virtual void mouseDown (Component* block, const MouseEvent& e) = 0;
        
        /**
         *  Mouse drag callback. Can call dragComponent to move the
         *  block passed.
         *  @param block is a pointer to the object to be manipulated.
         *  @param e is the mouse down event.
         */
        virtual void mouseDrag (Component* block, const MouseEvent& e) = 0;
        
        /**
         *  Getter for if this block has input.
         *  @return true if block has an input.
         */
        virtual const bool hasInput() = 0;
        
        /**
         *  Getter for the input area.
         *  @return a rectangle representing the input area.
         */
        virtual Rectangle<float> getInput();
        
        /**
         *  Getter for if this block has output.
         *  @return true if block has an output.
         */
        virtual const bool hasOutput() = 0;
        
        /**
         *  Getter for the output area.
         *  @return a rectangle representing the output area.
         */
        virtual Rectangle<float> getOutput();
        
        /**
         *  Getter for if this block is a parameter object.
         *  @return true if this block is a parameter object.
         */
        virtual const bool isParam() = 0;
        
        /**
         *  Getter for the param area.
         *  @return a rectangle representing the param collision area.
         */
        virtual Rectangle<float> getParam();
        
        /**
         *  Setter for setting the connection areas X & Y co-ordinates to match
         *  the blocks.
         *  @param the block parent for this manipulator (parse this).
         */
        virtual void setConnections (Component* block) = 0;
        
    protected:
        /** The constraining bounds the dragger can move. */
        ComponentBoundsConstrainer* constrainer;
        /** The dragger object to move the block. */
        ComponentDragger dragger;
        /** Reference to the blocks global sizing. */
        const BlockScaling& scaling;
    };
    
    //==========================================================================
    
    /**
     *  Infterface for handling the movements and connections of command blocks.
     */
    class CommandBlockManipulator    : public BlockManipulator
    {
    public:
        /**
         *  Constructor.
         *  @param scaling is the sizing information for the block object.
         */
        CommandBlockManipulator (BlockScaling& scaling);
        
        /**
         *  Mouse down callback. @see BlockManipulator
         *  @param block is a pointer to the object to be manipulated.
         *  @param e is the mouse down event.
         */
        void mouseDown (Component* block, const MouseEvent& e) override;
        
        /**
         *  Mouse drag callback. @see BlockManipulator
         *  @param block is a pointer to the object to be manipulated.
         *  @param e is the mouse down event.
         */
        void mouseDrag (Component* block, const MouseEvent& e) override;
        
        /**
         *  Getter for if this block has input.
         *  @return true for a command block.
         */
        const bool hasInput() override;
        
        /**
         *  Getter for input area.
         *  @return the input area rectangle.
         */
        Rectangle<float> getInput() override;
        
        /**
         *  Getter for if this block has output.
         *  @return true for a command block.
         */
        const bool hasOutput() override;
       
        /**
         *  Getter for output area.
         *  @return the output area rectangle.
         */
        Rectangle<float> getOutput() override;
        
        /**
         *  Getter for if this block is a parameter object.
         *  @return false for a command block.
         */
        const bool isParam() override;
        
        /**
         *  Setter for setting the connection areas X & Y co-ordinates to match
         *  the blocks.
         *  @param the block parent for this manipulator (parse this).
         */
        void setConnections (Component* block) override;

    private:
        /** The area for this blocks input. */
        Rectangle<float> input;
        /** The area for this blocks output. */
        Rectangle<float> output;
    };
    
    //==========================================================================
    
    /**
     *  Infterface for handling the movements and connections of trigger blocks.
     */
    class TriggerBlockManipulator    : public BlockManipulator
    {
    public:
        /**
         *  Constructor.
         *  @param scaling is the sizing information for the block object.
         */
        TriggerBlockManipulator (BlockScaling& scaling);
        
        /**
         *  Mouse down callback. @see BlockManipulator
         *  @param block is a pointer to the object to be manipulated.
         *  @param e is the mouse down event.
         */
        void mouseDown (Component* block, const MouseEvent& e) override;
        
        /**
         *  Mouse drag callback. @see BlockManipulator
         *  @param block is a pointer to the object to be manipulated.
         *  @param e is the mouse down event.
         */
        void mouseDrag (Component* block, const MouseEvent& e) override;
                
        /**
         *  Getter for if this block has input.
         *  @return false for a trigger block.
         */
        const bool hasInput() override;
        
        /**
         *  Getter for if this block has output.
         *  @return true for a trigger block.
         */
        const bool hasOutput() override;
        
        /**
         *  Getter for output area.
         *  @return the output area rectangle.
         */
        Rectangle<float> getOutput() override;
        
        /**
         *  Getter for if this block is a parameter object.
         *  @return false for a trigger block.
         */
        const bool isParam() override;
        
        /**
         *  Setter for setting the connection areas X & Y co-ordinates to match
         *  the blocks.
         *  @param the block parent for this manipulator (parse this).
         */
        void setConnections (Component* block) override;
        
    private:
        /** The area for this blocks output. */
        Rectangle<float> output;
    };
    
    //==========================================================================
    
    class FunctionBlockManipulator    : public BlockManipulator
    {
    public:
        /** 
         *  Constructor.
         *  @param scaling is the sizing information for the block object.
         */
        FunctionBlockManipulator (BlockScaling& scaling);
        
        /**
         *  Mouse down callback. @see BlockManipulator
         *  @param block is a pointer to the object to be manipulated.
         *  @param e is the mouse down event.
         */
        void mouseDown (Component* block, const MouseEvent& e) override;
        
        /**
         *  Mouse drag callback. @see BlockManipulator
         *  @param block is a pointer to the object to be manipulated.
         *  @param e is the mouse down event.
         */
        void mouseDrag (Component* block, const MouseEvent& e) override;
        
        /**
         *  Getter for if this block has input.
         *  @return false for a function block.
         */
        const bool hasInput() override;
        
        /**
         *  Getter for if this block has output.
         *  @return false for a function block.
         */
        const bool hasOutput() override;
        
        /**
         *  Getter for if this block is a parameter object.
         *  @return true for a function block.
         */
        const bool isParam() override;
        
        /**
         *  Getter for the param area.
         *  @return a rectangle representing the param collision area.
         */
        Rectangle<float> getParam() override;
        
        /**
         *  Setter for setting the connection areas X & Y co-ordinates to match
         *  the blocks.
         *  @param the block parent for this manipulator (parse this).
         */
        void setConnections (Component* block) override;
        
    private:
        /** The bounds for detecing the parameter block */
        Rectangle<float> paramBounds;
    };
    
} // namespace juckly
