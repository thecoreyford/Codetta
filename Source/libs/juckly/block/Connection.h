/*
  ==============================================================================

    Connection.h
    Created: 29 Aug 2018 5:47:37pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"

//==============================================================================

/** Main namespace for the juckly library */
namespace juckly
{
    /**
     *  An object for tracking doubly-linked nodes (e.g. blocks).
     */
    template<class Node>
    class Connection
    {
    public:
        /**
         *  Construtor. Initialises default values.
         */
        Connection()
        {
            startNodeStatus = false;
            previousNode = nullptr;
            nextNode = nullptr;
        }
        
        /**
         *  Destructor.
         */
        ~Connection()
        {
            // In this case nodes are deleted by disposing of the block objects,
            // hence why no ptr deletion as this would cause dangling pointers.
        }
        
        //======================================================================
        
        /**
         *  Getter for if this connection is a start node.
         *  @return true if node is a start node.
         */
        const bool& isStartNode() const { return startNodeStatus; }
        
        /**
         *  Setter for start node status. False by default. 
         *  @param if node should be a start node.
         */
        void setStartNode (bool isStartNode = false)
        {
            startNodeStatus = isStartNode;
        }
        
        /**
         *  Setter for previous node. 
         *  @param new previous node value.
         */
        void setPreviousNode (Node* node) { previousNode = node; }
        
        /**
         *  Getter for previous node.
         *  @return pointer to the current previous node object.
         */
        Node* getPreviousNode() const { return previousNode; }
        
        /**
         *  Setter for next node.
         *  @param new next node value.
         */
        void setNextNode (Node* node) { nextNode = node; }
        
        /**
         *  Getter for the next node.
         *  @return pointer to the current next node object.
         */
        Node* getNextNode() const { return nextNode; }
        
    private:
        /** If this node is a "head" node or not. */
        bool startNodeStatus;
        /** The previous node for this connection. */
        Node* previousNode;
        /** The next node for this connection. */
        Node* nextNode;
    };
} // namespace juckly 
