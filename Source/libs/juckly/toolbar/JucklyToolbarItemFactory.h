/*
  ==============================================================================

    JucklyToolbarItemFactory.h
    Created: 15 Aug 2018 3:32:54pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "IDTracker.h"
#include "ToolbarBlockIcon.h"

//==============================================================================

/** Main namespace for the juckly library */
namespace juckly
{
    /**
     *  Factory for generating block items for a juce toolbar.
     */
    class JucklyToolbarItemFactory    : public ToolbarItemFactory
    {
    public:
        
        /**
         *  Constructor. Sets the drag drop ptr to null.
         */
        JucklyToolbarItemFactory();
        
        /**
         *  Destructor. Currently does nothing.
         */
        ~JucklyToolbarItemFactory();
        
        /**
         *  Setter for the drag drop area.
         */
        void setDragDropArea (DragAndDropContainer* dragDropAreaRhs);
        
        /**
         *  Returns all unique ID's in the toolbar. 
         *  @param reference to array for each unique block ID value.
         */
        void getAllToolbarItemIds (Array <int>& ids) override;
        
        /**
         *  Returns all unique ID's avaliable in the toolbar as default.
         *  @param reference to array for each default block ID value.
         */
        void getDefaultItemSet (Array <int>& ids) override;
        
        /**
         *  Creates an item present in the toolbar as default.
         *  @param the unique ID for the given toolbar item. 
         *  @return the component (block) to be placed with the toolbar.
         */
        ToolbarItemComponent* createItem (int itemId) override;
    
    private:
        /** The container for the area icons can be dragged within. */
        DragAndDropContainer* dragDropArea;
    };
} // namespace juckly 
