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
#include "../data-logger/DataLogger.h"

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
            addMouseListener (this, true); // for data logging
            prevHorizontal = -1; // for data logging
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
        
        //======================================================================
        
        #ifdef USE_LOGGING
        void scrollBarMoved (ScrollBar* bar, double newRangeStart) override
        {
            if (bar->isVertical())
            {
                if (prevHorizontal != 0){
                    LOG_STRING ("vertical bar selected");
                    prevHorizontal = 0;
                }
            }
            else
            {
                if (prevHorizontal != 1){
                    LOG_STRING ("horizontal bar selected");
                    prevHorizontal = 1;
                }
            }
            
            // adapted from original callback in  juce library viewport class
            auto newRangeStartInt = roundToInt (newRangeStart);

            if (!bar->isVertical())
            {
                setViewPosition (newRangeStartInt, getViewPositionY());
            }
            else if (bar->isVertical())
            {
                setViewPosition (getViewPositionX(), newRangeStartInt);
            }
        }
        
        void mouseUp (const MouseEvent& event) override
        {
            prevHorizontal = -1;
        }
        #endif
        
        //======================================================================
        
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
        
    public:
        /** If the sliders previously been moved */
        int prevHorizontal; //< for data logging
    };
    
} // namespace codetta
