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
    
    void Toolbox::addBlock (const String& categoryName, const std::shared_ptr<Block>& block)
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
        
        // Add the block to our owned array for tracking
        blocks.push_back (block);
        
        // Add the block to our ID tracker
        IDTracker::getInstance().addBlock (blocks.back()->getID(),
                                           blocks.back()->getToolboxIcon());
        
        // Add that ID to the toolbar corresponding to this category
        toolbars[categoryID]->addItem(categorys[categoryID]->itemFactory,
                                      IDTracker::getInstance().getIntID (blocks.back()->getID()));
        
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
                LOG_STRING (newCurrentTabName + " tab selected");
            }
        }
    }
    
    void Toolbox::changeCategoryName (const String& oldCategoryName,
                                      const String& newCategoryName)
    {
        bool contained = false;
        for (int i = 0; i < categorys.size(); i++)
        {
            if (categorys[i]->name == oldCategoryName)
            {
                contained = true;
                categorys[i]->name = newCategoryName;
            }
        }
        
        // You are tyring to change the name of a category that dosen't exist!
        jassert (contained);
        
        updateTabs();
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
