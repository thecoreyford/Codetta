/*
  ==============================================================================

    BlockManipulatorFactory.h
    Created: 27 Aug 2018 11:27:41pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

/** Main namespace for the juckly library */
namespace juckly
{
    /**
     *  Enumeration of different block types.
     *  A detailed overview can be found here:  // REF: (Maloney et al., 2010)
     */
    enum class BlockType
    {
        commandBlock = 0,
        functionBlock = 1,
        triggerBlock = 2,
        //        controlStructure = 3
    };
    
    //==========================================================================
    
    /**
     *  Factory for creating block manipulator.
     *  Inspired by the model represented here:  // REF: (Gamma et al., 1994) pp. 107-116
     */
    class BlockManipulatorFactory
    {
    public:
        
        /**
         *  Creator function for different block manipulators. 
         *  @param the type of block from the BlockType enum.
         *  @param scaling is the sizing information for the block object.
         *  @return a unique pointer for the block manipulator object for the 
         *      enum value.
         */
        std::unique_ptr<BlockManipulator> createManipulator (BlockType blockType, BlockScaling& scaling)
        {
            switch (blockType) {
                case BlockType::commandBlock:
                    return std::make_unique<CommandBlockManipulator>(scaling);
                    break;
                case BlockType::triggerBlock:
                    return std::make_unique<TriggerBlockManipulator>(scaling);
                    break;
                case BlockType::functionBlock:
                    return std::make_unique<FunctionBlockManipulator>(scaling);
                    break;
                default:
                    // Please make sure that the block type parameter is selected
                    // from the enumeration at the top of this file.
                    jassert(false);
                    break;
            }
        }
    };
} // namespace juckly 
