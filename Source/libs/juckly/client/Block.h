/*
  ==============================================================================

    Block.h
    Created: 14 Aug 2018 1:18:40pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../../../data-logger/DataLogger.h"
#include "../block/BlockManipulator.h"
#include "../block/BlockManipulatorFactory.h"
#include "../block/Connection.h"
#include "../block/BlockScaling.h"
#include "../block/BlockGUI.h"
#include "../block/BlockParamHole.h"
#include "../block/Clipboard.h"
#include "BlockSettings.h"

//==============================================================================

#define CODETTA 0 //< add macro so that the libs folder can remain clean for
                  // potentially releasing as an API.
#ifdef CODETTA
    #include "../../../gui/widgets/ContextTracker.h"
    #include "../../../gui/widgets/UndoWidget.h"
#endif

//==============================================================================

//==============================================================================

/** Main namespace for the juckly library */
namespace juckly
{
    /**
     *  The class representing one block.
     */
    class Block    : public Component
    {
    protected:
        /**
         *  Long parametarised constructor. Has all objects needed to make a block.
         *  @see BlockSettings.h to see if an abstracted option is avaliable.
         *  @param uniqueID is this blocks identifier: used in @see BlockFactory.
         *  @param toolboxIconRhs is the image displayed in the toolbar.
         *  @param workspaceImageRhs is the image displayed behind the internalUI. 
         *  @param blockType is the way this block should interact with other blocks.
         *  @param isStartNodeRhs is true if this block should be a starting block.
         *  @param internalUIRhs is the component encased within this block: if nullptr,
         *             is passed no component is added.
         *  @param isGlobalRhs determins if this block is only for a one time call.
         *  @param takesParam is true if this block can accept a parameter.
         */
        Block (const String uniqueID,
               Image toolboxIconRhs,
               Image workspaceImageRhs,
               BlockType blockType,
               bool isStartNodeRhs,
               Component* iternalUIRhs,
               bool takesParamRhs,
               const int width = 130,
               const int height = 100,
               bool isGlobalRhs = false);
        
        /**
         *  Abstracted constructor. Passes a block setting @see BlockSettings.h
         *  @param a pointer to a settings object that defines all objects 
         *      needed to create a block.
         */
        Block (std::unique_ptr<BlockSettings> blockSettings);
        
    private:
        
        /**
         *  Function containing all setup code to be called by a constructor.
         *  @param blockType is the way this block should interact with other blocks.
         *  @param isStartNode is true if this block should be a starting block.
         *  @param the width for this block. 
         *  @param the height for this block.
         */
        void initialise (BlockType& blockType,
                         bool& isStartNode,
                         const int& width,
                         const int& height);
        
    public:
        /**
         *  Virtual Destructor. Deletes internalUI.
         */
        virtual ~Block();
        
        //======================================================================
        //  Manipulator Functions
        
        /**
         *  Sets constrainer within the manipulator @see manipulator.
         *  @param the constrainer object.
         */
        void setConstrainer (ComponentBoundsConstrainer* constrainerRhs);
        
        /**
         *  Callback for a mouse down event. 
         *  Manipulates the blocks movement and updates neighbouts.
         *  @param the mouse event.
         */
        void mouseDown (const MouseEvent& e) override;
        
        /**
         *  Callback for a mouse drag event. 
         *  Manipulates the blocks movement, hi-lights and updates neighbouts.
         *  @param the mouse event.
         */
        void mouseDrag (const MouseEvent& e) override;
        
        /**
         *  Callback for a mouse up event.
         *  Broadcasts to workspace to check block collisions.
         *  @param the mouse event.
         */
        void mouseUp (const MouseEvent& e) override;
        
        /**
         *  Getter for the manipulator object. 
         *  @return a unique ptr to the manipulator.
         */
        const std::unique_ptr<BlockManipulator>& getManipulator() const;
        
        /**
         *  Listener class for broadcasting messages.
         */
        class Listener
        {
        public:
            /** Virual Destructor */
            virtual ~Listener(){}
            
            /**
             *  Compares the passed block with others to find collisions.
             *  @param  block is the block that is moving (this).
             *  @param isMouseDrag is true if called from a mouseDrag callback.
             */
            virtual void checkCollisions (Block* block, bool isMouseDrag) = 0;
        };
        
        /**
         *  Setter for the listener.
         *  @param the new listener object reciving broadcasts.
         */
        void setListener (Listener* newListener);
        
        //======================================================================
        //  Connection Functions
        
        /**
         *  Getter for the block connections.
         *  @return the connections for this block.
         */
        Connection<Block>& getConnection();
        
        /**
         *  Moves all blocks connected to the left of this current block.
         */
        void moveNeighbours();
        
        //======================================================================
        //  Gui Functions
        
        /**
         *  Getter for the toolbox icon.
         *  @return the toolbox icon image.
         */
        const Image& getToolboxIcon() const;
        
        /**
         *  Draws GUI and background for the internal UI.
         *  @param the graphics context.
         */
        void paint (Graphics& g) override;
        
        /**
         *  Applys size changes if object size varies.
         */
        void resized() override;
        
        /**
         *  Getter for the block GUI.
         *  @return the block gui.
         */
        BlockGUI& getGui();
        
        /**
         *  Getter for the block scaling. 
         *  @return the block scaling.
         */
        BlockScaling& getScaling();
        
        //======================================================================
        //  Pure Viruals
        /**
         *  The code to be run when the block is reached.
         *
         *  To use functions within the internalUI it is reccomended to pass 
         *  the pointer as a param in the derived class which is assigned to 
         *  the constructor argument and a private variable.
         *  @see DemoBlocks.h DemoBlock object for an example.
         *
         */
        virtual void doAction() = 0;
        
        /** Wheather save or load is called. */
        enum FileManipulator
        {
            save,
            load
        };
        
        /**
         *  The code used to store information, and set information, for this block's internal UI.
         *
         *  Save information from your internal UI in an XML readable format, so that on load it can be
         *  updated using your internal components setters!
         *  @param head element for this block
         *  @param flag for whether the funtion is calling the save or load function.
         */
        virtual void doSaveOrLoad (XmlElement* blockHead, FileManipulator mode) = 0;
        
        //======================================================================
        //  Other
        
        /**
         *  Getter for the blocks unique identification. 
         *  @return the string ID for this block.
         */
        const String& getID() const;
        
        /**
         *  Getter for the internalUI component.
         *  @return internalUI component ptr.
         */
        const bool willTakeParam() const;
        
        /**
         *  Getter for block param. 
         *  @return the block param object.
         */
         BlockParamHole& getBlockParam();
        
        /**
         *  Getter for the internal component.
         *  @return the internal component.
         */
        Component*& getInternalUI();
        
        /**
         *  Getter for if the block is global.
         *  @return the blocks global state.
         */
        const bool& isGlobalState() const;
        
        /**
         *  Getter for if the block is start node.
         *  @return if the block is a start node or not.
         */
        const bool& isStartBlock() const;
        
        //======================================================================
        
        /**
         *  Set if this block is loading in or not.
         *  @param if the block is loading in or not.
         */
        void setLoading (bool newLoading);

        /**
         *  Getter for if this block is currently loading in or not.
         *  @return if the block is a loading in or not.
         */
        const bool& isLoadingIn() const;
       
    private:
        
        //======================================================================
        // Manipulator Variables
        /** Object managing all block manipulaton. */
        std::unique_ptr<BlockManipulator> manipulator;
        /** This blocks category type. */
        BlockType blockType;
        
        //======================================================================
        // Connection Variables
        /** Object managing connections between this block and its neigbours. */
        Connection<Block> connection;
        
        /** Listener pointer for broadcasts */
        Listener* listener;
        
        //======================================================================
        // Gui Variables
        /** Object managing block's basic GUI */
        BlockGUI gui;
        
        /** This blocks sizing */
        BlockScaling scaling;
        
        //======================================================================
        // Internal UI Variables
        /** Pointer to the internal UI component.*/
        Component* internalUI;
        /** The bounds for the internal UI component.*/
        Rectangle<float> internalUIBounds;
        /** True if component object is of type BlockParam */
        bool takesParam;
        /** The block parameter hole for accepting function blocks. */
        BlockParamHole blockParam;
        
        //======================================================================
        // Other Variables
        /** Unique identifier for the block. */
        const String blockID;
        
        /** Whether this block is a start node or not. */
        bool isStartNode;
        
        /** Determins if this block is a one use item */
        bool isGlobal;
        
        /** Wheather this block is  being loaded in or not. */
        bool loadingIn;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Block)
    };
} // namespace juckly 
