/*
  ==============================================================================

    DynamicsChanger.h
    Created: 1 Feb 2019 3:41:58pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../audio/Audio.h"
#include "../libs/juckly/client/Block.h"
#include "../data-logger/DataLogger.h"
#include "../gui/widgets/UndoWidget.h"
#include "BlockSlider.h"

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  A block that varies the tempo.
     */
    class DynamicsChanger    : public juckly::Block
    {
    public:
        /** Default constructor. */
        DynamicsChanger () : juckly::Block ( std::make_unique<juckly::CommandBlockSettings>
                                         ("DynamicsChanger",
                                          ImageCache::getFromMemory (BinaryData::dynamicsChangerIcon_png, BinaryData::dynamicsChangerIcon_pngSize),
                                          ImageCache::getFromMemory (BinaryData::darkPinkBlock_png, BinaryData::darkPinkBlock_pngSize),
                                          false /* not a start node */,
                                          new BlockSlider ("Dyn +", -64.0, 64.0, 10.0, 0.0, 0.0, 0.0, false),
                                          false /* dosen't take a parameter */
                                          )
                                         )
        {
           //TODO: guard for first time this is created!
           //TODO: just one slider moved variable
           auto slider = dynamic_cast<BlockSlider*>(getInternalUI());
           #ifdef USE_LOGGING
           prev = 10; //init variables
           slider->onValueChanged = [this]
           {
               // Update undo stack
//               codetta::UndoWidget::get().updateUndoStack();
               
               // Log info
               auto slider = dynamic_cast<BlockSlider*>(getInternalUI());
               if (prev > slider->getValue())
                   LOG_STRING (getID() + " slider decremented");
               else
                   LOG_STRING (getID() + " slider incremented");
               prev = slider->getValue();
           };
           #else
           slider->onValueChanged = [this]
           {
               codetta::UndoWidget::get().updateUndoStack();
           };
           #endif
        }
        
        /** Changes the tempo to the slider value. */
        void doAction() override
        {
            int currentVel = PlaybackSettings::get().getVelocity();
            
            // Increment the tempo to the slider value.
            auto increment = dynamic_cast<BlockSlider*>(getInternalUI());
            jassert (increment != nullptr); // Cast didn't work!
            if (increment != nullptr)
                PlaybackSettings::get().setVelocity (increment->getValue() + currentVel);
        }
        
        //======================================================================
        
        /**
         *  Contains info for saving and loading the dynamic changers internal data
         *  @param the head element for this block
         *  @param if save or load should be performed
         */
         void doSaveOrLoad (XmlElement* blockHead, FileManipulator mode) override
         {
             auto increment = dynamic_cast<BlockSlider*>(getInternalUI());
             
             if (mode == FileManipulator::save)
             {
                 blockHead->setAttribute ("dynamicsIncrement",
                                          increment->getValue());
             }
          
             //=================================================================
          
             if (mode == FileManipulator::load)
             {
                 increment->setValue (blockHead->getIntAttribute ("dynamicsIncrement"));
             }
         }
        
    private:
        #ifdef USE_LOGGING
        int prev;
        #endif
    };
    
} // namespace codetta
