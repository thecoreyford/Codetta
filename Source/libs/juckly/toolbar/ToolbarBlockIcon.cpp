/*
  ==============================================================================

    ToolbarBlockIcon.cpp
    Created: 18 Aug 2018 3:55:14pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "ToolbarBlockIcon.h"

/** Main namespace for the juckly library */
namespace juckly
{
    ToolbarBlockIcon::ToolbarBlockIcon (const int toolbarBlockId,
                                        DragAndDropContainer* dragDropAreaRhs,
                                        const String& blockID,
                                        const Image& iconImageRhs)
        : ToolbarItemComponent (toolbarBlockId, blockID, false),
          icon (iconImageRhs),
          iconID (blockID),
          dragDropArea (dragDropAreaRhs)
    {

    }
    
    ToolbarBlockIcon::~ToolbarBlockIcon(){}
    
    bool ToolbarBlockIcon::getToolbarItemSizes (int /*toolbarDepth*/,
                                                bool isVertical,
                                                int& preferredSize,
                                                int& minSize,
                                                int& maxSize)
    {
        // REF: (WeAreROLI, 2018)
        if (isVertical)
            return false;
        
        preferredSize = 200;
        minSize = 50;
        maxSize = 250;
        return true;
    }
    
    void ToolbarBlockIcon::paintButtonArea (Graphics& g, int width, int height, bool, bool)
    {
        Rectangle<float> area;
        area.setWidth  (width * 0.5);
        area.setHeight (height);

        g.drawImage (icon, area);
    }
    
    void ToolbarBlockIcon::contentAreaChanged (const Rectangle<int>& newArea){}
    
    void ToolbarBlockIcon::mouseDown (const MouseEvent& event)
    {
        // You must have setup your main component initialisation list improperly.
        // The main GUI containing the toolbar must inherit from DragAndDropContainer,
        // and initliase the toolbar with a pointer to "this".
        //     e.g. MainComponent::MainComponent() : toolbox(this)
        jassert(dragDropArea != nullptr);
        
        // REF: (Aros, 2015)
        
        // The entire main component (containing workspace and toolbar)
        // must be passed to this class in order for generated icons to
        // be draggable.
        // Just finding the parent object assumes the toolbar itself is
        // the draggable area.
        dragDropArea->startDragging(iconID, this);
    }
} // namespace juckly 
