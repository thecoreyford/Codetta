/*
 ==============================================================================
 
 IDTracker.cpp
 Created: 5 Sep 2018 4:01:58pm
 Author:  Corey Ford
 
 ==============================================================================
 */

#include "IDTracker.h"

/** Main namespace for the juckly library */
namespace juckly
{
    //======================================================================
    // Public Functions.
    
    IDTracker& IDTracker::getInstance()
    {
        static IDTracker instance;
        return instance;
    }
    
    void IDTracker::addBlock (const String& name, const Image& icon)
    {
        // You've already added a block with this ID!
        // Make sure you haven't added dupliate blocks.
//        jassert ( !idAlias.contains (name)); //  I want to be able to add duplicate blocks...
                                               //  ...same block in two places.
        
        currentUnusedID++; // move onto the next ID
        
        // Ensure that we don't apply an ID of zero.
        // This isn't a valid toolbar component ID number.
        jassert (currentUnusedID != 0);
        
        idAlias.set (name, currentUnusedID);
        iconList.set (currentUnusedID, icon);
    }
    
    int IDTracker::getIntID (const String& name) const
    {
        return idAlias[name];
    }
    
    
    const String IDTracker::getStringForID (const int ID) const
    {
        HashMap<const String, int>::Iterator iterator (idAlias);
        while (iterator.next())
        {
            if(iterator.getValue() == ID)
            {
                return iterator.getKey();
            }
        }
        
        // This string could not be found. Check your spellings and ensure
        // it has been added to the block list.
        jassert (false);
    }
    
    
    Array<int> IDTracker::getAllIntIDs() const
    {
        Array<int> ids;
        
        HashMap<const String, int>::Iterator iterator (idAlias);
        while (iterator.next())
        {
            ids.add ( iterator.getValue() );
        }
        
        return ids;
    }
    
    
    Image IDTracker::getToolboxIconForID (const int ID) const
    {
        return iconList[ID];
    }
    
    //======================================================================
    // Private Functions.
    
    /** Private constructor. Initialises first unused ID to zero. */
    IDTracker::IDTracker() { currentUnusedID = 0; }
    
    /** Private Destructor */
    IDTracker::~IDTracker(){}
    
    /**
     *  Private Copy Constructor
     *  @param copyable value
     */
    IDTracker::IDTracker (const IDTracker&) {}
    
    /**
     *  Private Copy Operator
     *  @param copyable value
     */
    void IDTracker::operator= (const IDTracker&) {}
    
} // namespace juckly
