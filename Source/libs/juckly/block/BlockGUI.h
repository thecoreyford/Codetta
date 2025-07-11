/*
  ==============================================================================

    BlockGUI.h
    Created: 30 Aug 2018 6:34:07pm
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
     *  Enumeration describing the current state of any connections intersecting.
     */
    enum OverlapState
    {
        noOverlap = 0,
        outputOverlap = 1,
        inputOverlap = 2,
        inOutOverlap = 3,
        paramOverlap = 4
    };
    
    //==========================================================================
    
    /**
     *  GUI details for block objects.
     */
    class BlockGUI
    {
    public:
       
        /**
         *  Constructor. Parses images and initialises overlap to noOverlap by
         *  default. 
         *  @param toolboxIconRhs is the image displayed in the toolbar.
         *  @param workspaceImageRhs is the image displayed behind the internalUI.
         *  @param scalingRhs is the sizing information for the block object.
         */
        BlockGUI (Image toolboxIconRhs,
                  Image workspaceImageRhs,
                  BlockScaling& scalingRhs);
        
        /**
         * Getter for the toolbox icon.
         * @return the image or the icon representation.
         */
        const Image& getToolboxIcon() const;
        
        /**
         *  Painting for the block component.
         *  @param g is the graphics context for the block.
         *  @param localBounds is the size of the original block passed.
         *  @param paintUI is true if a background should be drawn 
         *      for an internal component.
         *  @param internalUIBounds is the area of an internal components bounds.
         */
        void paint (Graphics& g,
                    Rectangle<int> localBounds,
                    bool paintUI,
                    Rectangle<float>& internalUIBounds);
        
        /** 
         *  Sets the overlap state depending on if this block collides with another.
         *  @param block is this block object - being dragged.
         *  @param newOverlapState is the new setting for the overlap state.
         */
        void setOverlapState(Component* block, OverlapState newOverlapState);
        
    private:
        /** Private Constructor */
        BlockGUI(); 
        
        /** The image representing the block in the toolbox. */
        Image toolboxIcon;
        /** The background of the block shown in the workspace. */
        Image workspaceImage;
        
        /** The overlap state for block collisons. */
        OverlapState overlapState;
        
        /** Reference to the blocks global sizing */
        BlockScaling& scaling;
    };
} // namespace juckly 
