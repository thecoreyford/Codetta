/*
  ==============================================================================

    BlockSettings.h
    Created: 4 Sep 2018 8:09:54pm
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
     *  A class with all objects needed to initialise a block object.
     */
    class BlockSettings
    {
    public:
        /**
         *  Constructor.
         *  @param uniqueIDRhs is this blocks identifier: used in @see BlockFactory.
         *  @param toolboxIconRhs is the image displayed in the toolbar.
         *  @param workspaceImageRhs is the image displayed behind the internalUI.
         *  @param blockTypeRhs is the way this block should interact with other blocks.
         *  @param isStartNodeRhs is true if this block should be a starting block.
         *  @param internalUIRhs is the component encased within this block: if nullptr,
         *             is passed no component is added.
         *  @param takesParamRhs is true if this block can accept a parameter.
         *  @param widthRhs is the horizontal size of a block in pixels.
         *  @param heightRhs is the vertical size of a block in pixels.
         *  @param isGlobalRhs is the blocks global state.
         */
        BlockSettings (const String uniqueIDRhs,
                      Image toolboxIconRhs,
                      Image workspaceImageRhs,
                      BlockType blockTypeRhs,
                      bool isStartNodeRhs,
                      Component* internalUIRhs,
                      bool takesParamRhs,
                      const int widthRhs = 130,
                      const int heightRhs = 100,
                      bool isGlobalRhs = false)
                        : uniqueID (uniqueIDRhs),
                          toolboxIcon (toolboxIconRhs),
                          workspaceImage (workspaceImageRhs),
                          blockType (blockTypeRhs),
                          isStartNode (isStartNodeRhs),
                          internalUI (internalUIRhs),
                          takesParam (takesParamRhs),
                          width (widthRhs),
                          height (heightRhs),
                          isGlobal (isGlobalRhs)
        
        {
        }
        
        /** Destructor */
        ~BlockSettings(){}
        
        /**
         * Getter for unique ID.
         * @returns the unique ID.
         */
        const String& getUniqueID() { return uniqueID; }

        /**
         * Getter for toolbox icon.
         * @returns the toolbox icon.
         */
        Image& getToolboxIcon() { return toolboxIcon; }

        /**
         * Getter for workspace image.
         * @returns the workspace image.
         */
        Image& getWorkspaceImage() { return workspaceImage; }

        /**
         * Getter for block type.
         * @returns the block type.
         */
        BlockType& getBlockType() { return blockType; }

        /**
         * Getter for isStartNode.
         * @returns true if block will be used as a start node.
         */
        bool& getIsStartNode() { return isStartNode; }

        /**
         * Getter for internal UI.
         * @returns the internal UI component.
         */
        Component*& getInternalUI() { return internalUI; }

        /**
         * Getter for takes parameter boolean.
         * @returns true if block will accept parameters.
         */
        bool& getTakesParam() { return takesParam; }
        
        /**
         * Getter for block width.
         * @returns the blocks width.
         */
        const int& getWidth() { return width; }
        
        /**
         * Getter for block height.
         * @returns the blocks height. 
         */
        const int& getHeight() { return height; }
        
        /**
         * Getter for the blocks global state.
         * @returns if this block is global or not.
         */
        const bool& getIsGlobal() { return isGlobal; }
        
    private:
        
        /** Private constructor */
        BlockSettings();
        
        /**Identifier for a new block.*/
        const String uniqueID;
        /** Image displayed in toolbar for a block */
        Image toolboxIcon;
        /** Image displayed behind the internal UI for a block */
        Image workspaceImage;
        /** How the block should interact with other blocks */
        BlockType blockType;
        /** True if block should be a starting node */
        bool isStartNode;
        /** Component encased within a block */
        Component* internalUI;
        /** True if a block can accept a parameter. */
        bool takesParam;
        /** The blocks horizontal sizing. */
        const int width;
        /** The blocks vertical sizing. */
        const int height;
        /** If the block is global (one time use) */
        bool isGlobal;
    };
    
    //==========================================================================
    
    /**
     *  A class with all objects needed to initialise a command block object.
     */
    class CommandBlockSettings    : public BlockSettings
    {
    public:
        /**
         *  Constructor.
         *  @param uniqueID is this blocks identifier: used in @see BlockFactory.
         *  @param toolboxIcon is the image displayed in the toolbar.
         *  @param workspaceImage is the image displayed behind the internalUI.
         *  @param isStartNode is true if this block should be a starting block.
         *  @param internalUI is the component encased within this block: if nullptr,
         *             is passed no component is added.
         *  @param takesParam is true if this block can accept a parameter.
         *  @param width is the horizontal size of a block in pixels.
         *  @param height is the vertical size of a block in pixels.
         */
        CommandBlockSettings (const String uniqueID,
                       Image toolboxIcon,
                       Image workspaceImage,
                       bool isStartNode,
                       Component* internalUI,
                      bool takesParam,
                      const int width = 130,
                      const int height = 100)
        : BlockSettings (uniqueID,
                         toolboxIcon,
                         workspaceImage,
                         BlockType::commandBlock,
                         isStartNode,
                         internalUI,
                         takesParam,
                         width,
                         height )
        {
            
        }
        
    private:
        /** Private constructor */
        CommandBlockSettings();
    };
    
    //==========================================================================
    
    /**
     *  A class with all objects needed to initialise a trigger block object.
     */
    class TriggerBlockSettings    : public BlockSettings
    {
    public:
        /**
         *  Constructor.
         *  @param uniqueIDRhs is this blocks identifier: used in @see BlockFactory.
         *  @param toolboxIconRhs is the image displayed in the toolbar.
         *  @param workspaceImageRhs is the image displayed behind the internalUI.
         *  @param isStartNodeRhs is true if this block should be a starting block.
         *  @param internalUIRhs is the component encased within this block: if nullptr,
         *             is passed no component is added.
         *  @param takesParamRhs is true if this block can accept a parameter.
         *  @param width is the horizontal size of a block in pixels.
         *  @param height is the vertical size of a block in pixels.
         */
        TriggerBlockSettings (const String uniqueID,
                              Image toolboxIcon,
                              Image workspaceImage,
                              bool isStartNode,
                              Component* internalUI,
                              bool takesParam,
                              const int width = 130,
                              const int height = 100)
        : BlockSettings (uniqueID,
                         toolboxIcon,
                         workspaceImage,
                         BlockType::commandBlock,
                         isStartNode,
                         internalUI,
                         takesParam,
                         width,
                         height)
        {
        }

    private:
        /** Private constructor */
        TriggerBlockSettings();
    };
    
    //==========================================================================
    
    
    /**
     *  A class with all objects needed to initialise a funcion block object.
     */
    class FunctionBlockSettings    : public BlockSettings
    {
    public:
        /**
         *  Constructor.
         *  @param uniqueIDRhs is this blocks identifier: used in @see BlockFactory.
         *  @param toolboxIconRhs is the image displayed in the toolbar.
         *  @param workspaceImageRhs is the image displayed behind the internalUI.
         *  @param isStartNodeRhs is true if this block should be a starting block.
         *  @param internalUIRhs is the component encased within this block: if nullptr,
         *             is passed no component is added.
         *  @param width is the horizontal size of a block in pixels.
         *  @param height is the vertical size of a block in pixels.
         */
        FunctionBlockSettings (const String uniqueID,
                               Image toolboxIcon,
                               Image workspaceImage,
                               bool isStartNode,
                               Component* internalUI,
                               const int width = 130,
                               const int height = 100)
        : BlockSettings (uniqueID,
                         toolboxIcon,
                         workspaceImage,
                         BlockType::functionBlock,
                         isStartNode,
                         internalUI,
                         false,
                         width,
                         height)
        {
        }

    private:
        /** Private constructor */
        FunctionBlockSettings();
    };
    
    //==========================================================================
    
    /**
     *  A class with all objects needed to initialise a block which takes a 
     *  single parameter (function block) as input.
     */
    class ParametarisedBlockSettings    : public BlockSettings
    {
    public:
        /**
         *  Constructor.
         *  @param uniqueIDRhs is this blocks identifier: used in @see BlockFactory.
         *  @param toolboxIconRhs is the image displayed in the toolbar.
         *  @param workspaceImageRhs is the image displayed behind the internalUI.
         *  @param blockTypeRhs is the way this block should interact with other blocks.
         *  @param isStartNodeRhs is true if this block should be a starting block.
         *  @param width is the horizontal size of a block in pixels.
         *  @param height is the vertical size of a block in pixels.
         */
        ParametarisedBlockSettings (const String uniqueID,
                                    Image toolboxIcon,
                                    Image workspaceImage,
                                    BlockType blockType,
                                    bool isStartNode,
                                    const int width = 130,
                                    const int height = 100)
        : BlockSettings (uniqueID,
                         toolboxIcon,
                         workspaceImage,
                         blockType,
                         isStartNode,
                         nullptr,
                         true,
                         width,
                         height)
        {
        }
    
    private:
        /** Private constructor */
        ParametarisedBlockSettings();
    };
    
    //==========================================================================
    
    /**
     *  A class with all objects needed to initialise a global function block object.
     */
    class GlobalBlockSettings    : public BlockSettings
    {
    public:
        /**
         *  Constructor.
         *  @param uniqueIDRhs is this blocks identifier: used in @see BlockFactory.
         *  @param toolboxIconRhs is the image displayed in the toolbar.
         *  @param workspaceImageRhs is the image displayed behind the internalUI.
         *  @param internalUIRhs is the component encased within this block: if nullptr,
         *             is passed no component is added.
         *  @param width is the horizontal size of a block in pixels.
         *  @param height is the vertical size of a block in pixels.
         */
        GlobalBlockSettings (const String uniqueID,
                               Image toolboxIcon,
                               Image workspaceImage,
                               Component* internalUI,
                               const int width = 130,
                               const int height = 100)
        : BlockSettings (uniqueID,
                         toolboxIcon,
                         workspaceImage,
                         BlockType::functionBlock,
                         true, //< all globals must be start nodes
                         internalUI,
                         false,
                         width,
                         height,
                         true) //< block is now global
        {
        }
        
    private:
        /** Private constructor */
        GlobalBlockSettings();
    };
    
} // namespace juckly 
