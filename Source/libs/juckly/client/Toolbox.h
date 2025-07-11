/*
  ==============================================================================

    Toolbox.h
    Created: 15 Aug 2018 2:44:00pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../../../data-logger/DataLogger.h"
#include "../toolbar/JucklyToolbarItemFactory.h"
#include "../toolbar/IDTracker.h"

//==============================================================================

/** Main namespace for the juckly library */
namespace juckly
{
    /**
     *  A container for adding and categorising your applications blocks.
     */
    class Toolbox    : public TabbedComponent
    {
    public:
        /**
         *  Constructor. Sets tabs to be at the top of application horizontally.
         *  @param  The area blocks can be dragged (both toolbox & workspace), 
         *          usually 'this' from the parent component.
         */
        Toolbox (DragAndDropContainer* dragDropAreaRhs);

        /** Destructor. */
        ~Toolbox();
        
        /**
         *  Creates a new category tab on the toolbar. 
         *  @param the category name visible on the tab.
         *  @param the colour assigned to this category.
         */
        void addCategory (const String& categoryName, const Colour& colour);
        
        /**
         *  Adds a block to a toolbox's category. 
         *  @param the category literal where the block should be placed.
         *  @param the block to be incorporated.
         */
        void addBlock (const String& categoryName, const std::shared_ptr<Block>& block);

        /**
         *  Callback for each change of tab. Ensures the corresponding toolbar 
         *      is visible for the selected category.
         *  @param the index for the current category.
         *  @param the string ID for the current category.
         */
        void currentTabChanged (int newCurrentTabIndex,
                                const String& newCurrentTabName) override;
        
        /**
         *  Rename function for categorys.
         *  @param old category name to be changed.
         *  @param the new category name.
         */
        void changeCategoryName (const String& oldCategory,
                                 const String& newCategory);
        
    private:
        /**
         *  Private constructor. You must pass the drag drop area.
         */
        Toolbox();
        
        /** The area blocks can be dragged (both toolbox & workspace).*/
        DragAndDropContainer* dragDropArea;
        
        /**
         *  Encapsulation of variables representing a single category.
         */
        struct Category
        {
            /** A categorys unique name. */
            String name = "TBC";
            /** A categorys colour. */
            Colour colour = Colours::red;
            /** The item factory for this category. */
            JucklyToolbarItemFactory itemFactory;
        };

        /**
         *  Makes all current categories added visible as tabs.
         */
        void updateTabs();
        
        /** Contains each category object. */
        OwnedArray<Category> categorys;
        
        /** Contains each toolbar object. */
        OwnedArray<Toolbar> toolbars;
        
        std::vector<std::shared_ptr<Block>> blocks;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Toolbox)
    };
    
} // namespace juckly 
