/*
  ==============================================================================

    BlockScaling.h
    Created: 30 Aug 2018 3:58:39pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"

//==============================================================================

/** Main namespace for the juckly library */
namespace juckly
{
    /** Wrapper for block size information. */
    class BlockScaling
    {
    public:
        
        /** Private constructor. Initialises private variables to defaults. */
        BlockScaling()
        {
            width = 130;
            height = 100;
            componentBoarder = 10;
            connectionWidthScaling = 0.12;
        }
        
        /** Private Destructor */
        ~BlockScaling(){}
        
        //======================================================================
        // Getters
        
        /**
         *  Getter for block width.
         *  @return the blocks width.
         */
        const int& getWidth() const { return width; }
        
        /**
         *  Getter for block height.
         *  @return the blocks height.
         */
        const int& getHeight() const { return height; }
        
        /**
         *  Getter for the components boarder size.
         *  @return the blocks boarder.
         */
        const int& getComponentBoarder() const { return componentBoarder; }
        
        /**
         *  Getter for the connection width. 
         *  @return the width for conneciton areas.
         */
        const int getConnectionWidth() const { return width * connectionWidthScaling; }
        
        /**
         *  Getter for the connection width scaling value.
         *  @return the connection width scaling value.
         */
        const float& getConnectionWidthScaling() const { return connectionWidthScaling; }

        //======================================================================
        // Setters
        
        /**
         *  Setter for the blocks width.
         *  @param the desired block width.
         */
        void setWidth (const int& newWidth) { width = newWidth; }

        /**
         *  Setter for the blocks height.
         *  @param the desired block height.
         */
        void setHeight (const int& newHeight) { height = newHeight; }
        
        /**
         *  Setter for the component boarder size.
         *  @param the desired component boarder size.
         */
        void setComponentBoarder(const int& newComponentBoarder)
        {
            componentBoarder = newComponentBoarder;
        }

        /**
         *  Setter for the connection width scaling factor.
         *  @param the desired connection width scaling factor.
         */
        void setConnectionWidthScaling (const float& newConnectionWidthScaling)
        {
            connectionWidthScaling = newConnectionWidthScaling;
        }

    private:
        //======================================================================
        // Private Functions
        
        //======================================================================
        // Private Variables
        /** Default width for a block */
        int width;
        /** Default height for a block */
        int height;
        /** Scaling for the width of connection area */
        float connectionWidthScaling;
        /** Width of spacing around internal UI components */
        int componentBoarder;
    };
} // namespace juckly
