/*
  ==============================================================================

    DemoBlocks.h
    Created: 19 Aug 2018 8:15:10pm
    Author:  Corey Ford
 
    @Description 
    This class shows a few demo blocks that have been derived from the classes
    within juckly. 
    
    The first two have been made by passing all parameters to the constructor, 
    whereas the other two have been made using objects from the @see BlockSettings
    class.
 
    These are a good source of inspiration for when creating your own 
    blocks - and show different ways to interact with components passed as 
    parameters, or as internal UI's.
 
  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../client/Block.h"
#include "BlockParamHole.h"
#include "../client/BlockSettings.h"

//==============================================================================

/** Main namespace for the juckly library */
namespace juckly
{
    /**
     *  A block demonstrating how to setup a basic comand block with a slider,
     *  inheriting from the block base class @see Block.
     */
    class DemoBlock    : public Block
    {
    public:
        /** Default constructor - creates the block without a slider, passing nullptr. */
        DemoBlock () : Block ("DemoBlock",
                             ImageCache::getFromMemory (BinaryData::demoSliderIcon_png, BinaryData::demoSliderIcon_pngSize),
                             ImageCache::getFromMemory (BinaryData::blankGreen_png, BinaryData::blankGreen_pngSize),
                             BlockType::commandBlock,
                             false,
                             nullptr,
                             false
                             ) { slider = nullptr; }
        
        /**
         *  Parametarised constructor - creates slider passed as parameter.
         *  @param a new slider object @see derived factory in MainComponent.h
         */
        DemoBlock (Slider* sliderRhs)
                    : Block ("DemoBlock",
                             ImageCache::getFromMemory (BinaryData::demoSliderIcon_png, BinaryData::demoSliderIcon_pngSize),
                             ImageCache::getFromMemory (BinaryData::blankGreen_png, BinaryData::blankGreen_pngSize),
                             BlockType::commandBlock,
                             false,
                             sliderRhs,
                             false
                             ),
                    slider (sliderRhs) { }
        
        /** Prints the slider value when called. */
        void doAction() override
        {
            if (slider != nullptr)
                DBG ("Write slider value -> " + (String)slider->getValue());
        }
        
        /**
         *  Contains dummy info for saving and loading block info using XMLElement
         *  @param the head element for this block
         *  @param if save or load should be performed
         */
        void doSaveOrLoad (XmlElement* blockHead, FileManipulator mode) override
        {
            DBG ("No save for load for block: " + getID());
        }
        
    private:
        /** 
         *  Pointer to the slider parsed, so it's members can be accessed
         *  by the doAction function.
         */
        Slider* slider;
    };
    
    //==========================================================================
    
    /**
     *  A block demonstrating how to setup a basic start (trigger) block, inheriting 
     *  from the block base class @see Block.
     */
    class DemoStartBlock    : public Block
    {
    public:
        /** Default constructor - creates a start block, passing true as the start param. */
        DemoStartBlock()
            : Block ("DemoStartBlock",
                     ImageCache::getFromMemory (BinaryData::startBlankRed_png, BinaryData::startBlankRed_pngSize),
                     ImageCache::getFromMemory (BinaryData::startBlankRed_png, BinaryData::startBlankRed_pngSize),
                     BlockType::triggerBlock,
                     true,
                     nullptr,
                     false
                     ) { }
        
        /** Alerts that this block was checked in the chain. */
        void doAction() override
        {
            DBG ("Start block checked!");
        }
        
        /**
         *  Contains dummy info for saving and loading block info using XMLElement
         *  @param the head element for this block
         *  @param if save or load should be performed
         */
        void doSaveOrLoad (XmlElement* blockHead, FileManipulator mode) override
        {
            DBG ("No save for load for block: " + getID());
        }
    };
    
    //==========================================================================
    
    /**
     *  A block demonstrating how to create a parameterised component.
     */
    class DemoFunctionBlock  : public Block
    {
    public:
        /** Default constructor - creates a FunctionBlock, passing nullptr as the internal component. */
        DemoFunctionBlock()
        : Block ( std::make_unique<FunctionBlockSettings> ("DemoFunctionBlock",
                                                           ImageCache::getFromMemory (BinaryData::functionBlankBlue_png, BinaryData::functionBlankBlue_pngSize),
                                                           ImageCache::getFromMemory (BinaryData::functionBlankBlue_png, BinaryData::functionBlankBlue_pngSize),
                                                           false,
                                                           nullptr,
                                                           80,
                                                           80
                                                           )
                 )
        {
            text = nullptr;
        }
        
        /**
         *  Parametarised constructor - creates a text editor.
         *  @param a new text editor object @see derived factory in MainComponent.h
         */
        DemoFunctionBlock(TextEditor* textRhs)
        : Block ( std::make_unique<FunctionBlockSettings> ("DemoFunctionBlock",
                                                           ImageCache::getFromMemory (BinaryData::functionBlankBlue_png, BinaryData::functionBlankBlue_pngSize),
                                                           ImageCache::getFromMemory (BinaryData::functionBlankBlue_png, BinaryData::functionBlankBlue_pngSize),
                                                           false,
                                                           textRhs,
                                                           80,
                                                           80
                                                           )
                 ) , text(textRhs) {}
        
        /** Reads out textbox contents. */
        void doAction() override
        {
            if (text != nullptr)
                DBG (text->getText());
        }
        
        /**
         *  Contains dummy info for saving and loading block info using XMLElement
         *  @param the head element for this block
         *  @param if save or load should be performed
         */
        void doSaveOrLoad (XmlElement* blockHead, FileManipulator mode) override
        {
            DBG ("No save for load for block: " + getID());
        }

    private:
        /**
         *  Pointer to the text editor parsed, so it's members can be accessed
         *  by the doAction function.
         */
        TextEditor* text;
        
    };
    
    //==========================================================================
    
    /**
     *  A block demonstrating how hold a parameter object. This class also shows 
     *  how to access derived components using only one constructor.
     */
    class DemoFunctionBlockHolder  : public Block
    {
    public:
        
        /** Constructor - made using block setting as to only worry about cruital parameters. */
        DemoFunctionBlockHolder()
        : Block( std::make_unique<ParametarisedBlockSettings> ("DemoFunctionBlockHolder",
                                                               ImageCache::getFromMemory (BinaryData::demoFunctionHolder_png, BinaryData::demoFunctionHolder_pngSize),
                                                               ImageCache::getFromMemory (BinaryData::blankGreen_png, BinaryData::blankGreen_pngSize),
                                                               BlockType::commandBlock,
                                                               false
                                                               )
                )
        { }
        
        /**
         *  Calls to get block param for its internal value - decided with a dynamic cast
         *  Only ever do this as long as you can guarentte that the dynamic cast will be
         *    of that type.
         */
        void doAction() override
        {
            auto param = getBlockParam().getInternalParam();
            param =  dynamic_cast<Block*>(param);
            if(param != nullptr)
                dynamic_cast<Block*>(param)->doAction();
            
            DBG ("Demo function block holder checked!");
        }
        
        /**
         *  Contains dummy info for saving and loading block info using XMLElement
         *  @param the head element for this block
         *  @param if save or load should be performed
         */
        void doSaveOrLoad (XmlElement* blockHead, FileManipulator mode) override
        {
            DBG ("No save for load for block: " + getID());
        }
    };
} // namespace juckly
