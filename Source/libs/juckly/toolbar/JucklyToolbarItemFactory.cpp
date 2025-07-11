/*
  ==============================================================================

    JucklyToolbarItemFactory.cpp
    Created: 15 Aug 2018 3:32:54pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "JucklyToolbarItemFactory.h"

/** Main namespace for the juckly library */
namespace juckly
{
    JucklyToolbarItemFactory::JucklyToolbarItemFactory()
    {
        dragDropArea = nullptr;
    }
    
    JucklyToolbarItemFactory::~JucklyToolbarItemFactory() { }
    
    void JucklyToolbarItemFactory::setDragDropArea (DragAndDropContainer* dragDropAreaRhs)
    {
        // For why this is needed @see ToolbarBlockIcon.cpp
        dragDropArea = dragDropAreaRhs;
    }
    
    void JucklyToolbarItemFactory::getAllToolbarItemIds (Array <int>& ids)
    {
        // Return all potential ID's from the block list singleton.
        ids = IDTracker::getInstance().getAllIntIDs();
    }
    
    void JucklyToolbarItemFactory::getDefaultItemSet (Array <int>& ids)
    {
        // Return all potential ID's from the block list singleton.
        // So as default ALL blocks will fill a single toolbar.
        ids = IDTracker::getInstance().getAllIntIDs();
    }
    
    ToolbarItemComponent* JucklyToolbarItemFactory::createItem (int itemId)
    {
        //The itemID should never be zero as this represents a null state.
        jassert( itemId != 0);
        
        return new ToolbarBlockIcon (itemId,
                                     dragDropArea,
                                     IDTracker::getInstance().getStringForID (itemId),
                                     IDTracker::getInstance().getToolboxIconForID (itemId));
    }
    
} // namespace juckly
