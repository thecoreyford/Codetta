/*
  ==============================================================================

    CodettaViewport.h
    Created: 27 Nov 2018 10:50:50am
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "../libs/juckly/client/Workspace.h"

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Viewport housing a juckly workspace for updating codetta.
     */
    class CodettaViewport    : public Viewport
    {
    public:
        /**
         *  Constructor.
         */
        CodettaViewport()
        {
            setViewedComponent (new juckly::Workspace());
            setScrollBarThickness (30);
        }
        
        /** Destructor */
        ~CodettaViewport(){}
        
        /**
         *  Getter for the internal workspace.
         *  @return a pointer to the workspace.
         */
        juckly::Workspace* getWorkspace() const
        {
            auto workspacePtr = dynamic_cast<juckly::Workspace*>(getViewedComponent());
            jassert (workspacePtr != nullptr); //< the viewport didn't cast to a Workspace, check ptrs!!!
            return workspacePtr;
        }
        
        /**
         *  Callback for when the area visible in the viewport changes.
         *  Used to update bin position within juckly @see workspace.h
         *  @param the new area visible within the viewport.
         */
        void visibleAreaChanged (const Rectangle<int>& newVisibleArea) override
        {
            auto workspacePtr = getWorkspace();
            workspacePtr->setVisibleBounds (getViewArea());
            workspacePtr->resized();
        }
    };
    
} // namespace codetta

