/*
  ==============================================================================

    ToolbarBlockIcon.h
    Created: 18 Aug 2018 3:55:14pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "IDTracker.h"

//==============================================================================

/** Main namespace for the juckly library */
namespace juckly
{
    /**
     *  A component representing a block's icon for a juce toolbar.
     */
    class ToolbarBlockIcon    : public ToolbarItemComponent
    {
    public:
        /**
         *  Constructor. 
         *  @param Unique numeric ID for the Juce ToolbarItemComponent
         *  @param The area the icon can be dragged within.
         *  @param The corresponding unique string block ID.
         *  @param The icon image component.
         */
        ToolbarBlockIcon (const int toolbarBlockId,
                          DragAndDropContainer* dragDropAreaRhs,
                          const String& blockID,
                          const Image& iconImageRhs);
        
        /**
         *  Destructor. Does nothing.
         */
        ~ToolbarBlockIcon();
        
        /**
         *  Sets default measurments for the icons size.
         *  @param toolbarDepth is the thickness for the toolbar.
         *  @param isVertial is the orientation of the toolbar.
         *  @param preferredSize is the size an icon would ideally be (between
         *      min and max values).
         *  @param minSize is the smallest an item can be
         *  @param maxSize is the largest an item can be
         *  @return true to implement.
         */
        bool getToolbarItemSizes (int /*toolbarDepth*/,
                              bool isVertical,
                              int& preferredSize,
                              int& minSize,
                              int& maxSize) override;
        
        /**
         *  Draws the icon image to the graphics context.
         *  @param g is the graphics context.
         *  @param width in pixels
         *  @param height in pixels
         *  @param for if mouse is over - irrelevant for this implementaion
         *  @param for if mouse is down - irrelevant for this implemenetation
         */
        void paintButtonArea (Graphics& g, int width, int height, bool, bool) override;
        
        /**
         *  Virtual doing nothing. For when the area varies.
         *  @param indicates the size of a potential new area.
         */
        void contentAreaChanged (const Rectangle<int>& newArea) override;
        
        /**
         *  Mouse down callback, starts dragging of this icon.
         *  @param information about the mouse down event.
         */
        void mouseDown (const MouseEvent& event) override; 
        
    private:
        /** Private constructor. */
        ToolbarBlockIcon();
        
        /** The image for this icon. */
        Image icon;
        
        /** The block name (ID) relating to this icon. */
        String iconID;
        
        /** The area in which this icon can be draggable. */
        DragAndDropContainer* dragDropArea;
    };
} // namespace juckly 
