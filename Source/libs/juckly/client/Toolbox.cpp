/*
  ==============================================================================

    Toolbox.cpp
    Created: 15 Aug 2018 2:44:00pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "Toolbox.h"

/** Main namespace for the juckly library */
namespace juckly
{
    Toolbox::Toolbox (DragAndDropContainer* dragDropAreaRhs)
        : TabbedComponent (TabbedButtonBar::TabsAtTop),
          dragDropArea(dragDropAreaRhs)
    {
        
    }
    
    Toolbox::~Toolbox(){}
    
    void Toolbox::addCategory (const String& name, const Colour& colour)
    {
        // This category allready exists!
        // Check all your category names and remove any duplicates.
        for(int i = 0; i < categorys.size(); ++i)
            if(categorys[i]->name == name) { jassert(false); }
        
        // Create the new category & toolbar
        categorys.add (new Category);
        categorys.getLast()-> name = name;
        categorys.getLast()-> colour = colour;
        categorys.getLast()-> itemFactory.setDragDropArea (dragDropArea);
        
        //create the corresponding toolbar
        toolbars.add (new Toolbar);
        
        // Update the tabs on the GUI.
        updateTabs();
    }
    
    void Toolbox::addBlock (const String& categoryName, const Block* block)
    {
        // find category by it's name
        int categoryID = -1;
        for(int i = 0; i < categorys.size(); i++)
        {
            if( categorys[i]->name == categoryName )
            {
                categoryID = i;
                break;
            }
        }
        if(categoryID == -1)
        {
            // You tried to add a block to a category that doesn't exist!
            // Check all your category parameters used to add blocks.
            jassert(false);
        }
        
        
        // Add the block to our ID tracker
        IDTracker::getInstance().addBlock (block->getID(), block->getToolboxIcon());
        
        // Add that ID to the toolbar corresponding to this category
        toolbars[categoryID]->addItem(categorys[categoryID]->itemFactory,
                                      IDTracker::getInstance().getIntID (block->getID()));
        
        updateTabs();
    }
    
    void Toolbox::currentTabChanged (int newCurrentTabIndex,
                                     const String& newCurrentTabName)
    {
        // Check through all categorys.
        for(int i = 0; i < categorys.size(); ++i)
        {
            // Make the correct toolbar visible for the current category.
            if(i != newCurrentTabIndex)
            {
                toolbars[i]->setVisible (false);
            }
            else
            {
                toolbars[i]->setVisible (true);
            }
        }
    }
    
    void Toolbox::updateTabs()
    {
        // empty the bar and any existing toolbars to start with
        clearTabs();

        // for each category
        for(int i = 0; i < categorys.size(); ++i)
        {
            // add the tab
            addTab (categorys[i]->name, categorys[i]->colour, toolbars[i], false);
        }
    }
} // namespace juckly 
