/*
  ==============================================================================

    Arcs.h
    Created: 19 Dec 2019 2:43:50pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//TODO: comment up this bad boy

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Layer for drawing arcs to the screen, demonstrating iterations. 
     */
    class Arcs    : public Component,
                    public Timer
    {
    public:
        /**
         *  Singleton Getter
         *  @return this logger object!
         */
        static Arcs& get()
        {
            static Arcs arcs;
            return arcs;
        }
        
        /** Destructor */
        ~Arcs();
       
        /** Struct of all the arc information */
        struct ArcInfo
        {
            int y = 0;
            int startX = 0;
            int endX = 0;
            float transparency = 1.0f;
        };
        
       /**
        *  Paint method.
        *  @param the graphics context for this object.
        */
        void paint (Graphics&) override;
              
        /** Resized function. */
        void resized() override;
        
        /** Fire off an arc */
        void triggerArc (int y, int startX, int endX);
        
        /** Timer callback for  fading in and out arcs */
        void timerCallback() override;
        
        /** Stops and clears all arcs on screen. */
        void stopDrawing();
        
    private:
        /** Constructor. */
        Arcs(){setInterceptsMouseClicks(false, false);}
        
        /** Array of active arcs. */
        Array<ArcInfo> arcs;
    };
} // namespace codetta
