/*
  ==============================================================================

    IDTracker.h
    Created: 15 Aug 2018 6:34:05pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../client/Block.h"

//==============================================================================

/** Main namespace for the juckly library */
namespace juckly
{
    /**
     *  A tracker of unique IDs & icons for all blocks added to every toolbox.
     */
    class IDTracker
    {
    public:
        
        /**
         *  Acessor for the block list. 
         *  @return the reference to the static block list object.
         */
        static IDTracker& getInstance();
        
        /**
         *  Adds the string ID to the list and assigned a unique numerical ID.
         *  @param the block's current string ID value.
         */
        void addBlock (const String& name, const Image& icon);
        
        /**
         *  Returns the unique ID value for the input block String ID.
         *  @param the blocks current string ID value.
         *  @return the unique numerical ID value for the block.
         */
        int getIntID (const String& name) const;
        
        /**
         *  Finds the string value for a given numerical ID value.
         *  @param the unique numerical ID value for a block.
         *  @return the string ID value.
         */
        const String getStringForID (const int ID) const;
        
        /**
         *  Getter for all ID values avaliable within the application.
         *  @return an array of all possible ID values.
         */
        Array<int> getAllIntIDs() const;
        
        /**
         *  Getter for the toolbox icon image based on block id.
         */
        Image getToolboxIconForID (const int ID) const;
        
    private:
        //======================================================================
        // Functions.
        
        /** Private constructor. Initialises first unused ID to zero. */
        IDTracker();
        
        /** Private Destructor */
        ~IDTracker();
        
        /** 
         *  Private Copy Constructor
         *  @param copyable value
         */
        IDTracker (const IDTracker&);
        
        /**
         *  Private Copy Operator
         *  @param copyable value
         */
        void operator= (const IDTracker&);
        
        //======================================================================
        // Variables.
        
        /** Hash map for allocating unique numerical ID values to block strings.*/
        HashMap<const String, int> idAlias;
                
        /** Hash map allocating ID's to their icon image. */
        HashMap<int, Image> iconList;
        
        /** The next avaliable unused ID number. */
        int currentUnusedID;
    };
} // namespace juckly 
