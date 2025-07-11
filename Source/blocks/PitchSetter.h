/*
  ==============================================================================

    PitchSetter.h
    Created: 17 Sep 2019 8:56:35pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../audio/Audio.h"
#include "../libs/juckly/client/Block.h"
#include "../data-logger/DataLogger.h"
#include "BlockSlider.h"

/** Main namespace for codetta. */
namespace codetta
{
    
    /**
     *  Internal UI component for the pitch setter block.
     */
    class PitchSetterComponent    : public Component
    {
    public:
        /**
         *  Constructor.
         */
        PitchSetterComponent()
            : tuningSlider("tuning", -7.0, 14.0, 0, 0.8, -12)
        {
            addAndMakeVisible (tuningSlider);
            icon = ImageCache::getFromMemory (BinaryData::tuningfork_png,
                                              BinaryData::tuningfork_pngSize);
            
            #ifdef USE_LOGGING
            tuningSlider.onValueChanged = [this]
            {
                // Update undo stack
//                codetta::UndoWidget::get().updateUndoStack();
                
                // Print information
                auto prev = PlaybackSettings::get().getTuningOffset();
                if (prev > tuningSlider.getValue())
                    LOG_STRING ("Pitch-setter slider decremented");
                else
                    LOG_STRING ("Pitch-setter slider incremented");
            };
            #else
            tuningSlider.onValueChanged = [this]
            {
                codetta::UndoWidget::get().updateUndoStack();
            };
            #endif
        }
        
        /** Destructor. */
        ~PitchSetterComponent(){}
        
        /**
         *  Paint callback.
         *  @param the graphics context and draws images for this object.
         */
        void paint (Graphics& g) override
        {
            //TODO: hardcoding bad
//            g.fillAll (Colour::fromRGB(2,226,227));
//            g.fillAll (Colours::turquoise);
            g.fillAll (Colours::white);
            
            // for the tuning fork icon...
            Rectangle<float> pitchforkRect;
            pitchforkRect.setWidth (getWidth() * 0.5f);
            pitchforkRect.setHeight (getHeight() * 0.60f);
            pitchforkRect.setY (15);
            g.drawImage (icon, pitchforkRect);
            
            // for the equals sign
            g.setColour (Colours::black);
            Rectangle<int> equalsRect = getLocalBounds();
            equalsRect.setX (26);
            equalsRect.setY (-10); //TODO: hardcoding bad!
            g.drawText ("=", equalsRect, Justification::left);
//
//            g.setFont (MusiSyncEng::NewFont::getNotationFont().withHeight (getHeight() * 1.25));
//            g.setColour (Colours::black);
//            g.drawText ("}", 0, 0, getWidth() * 0.50, getHeight(),
//                        Justification::left, false);
            
            
        }
        
        /** Resized Callback. Sets bounds of child components. */
        void resized() override
        {
            tuningSlider.setBounds (getLocalBounds());
        }
        
        /** Allow the pitch setter block to get the tuning.*/
        friend class PitchSetter;
        
    private:
        /** Slider component changing the tuning. */
        BlockSlider tuningSlider;
        
        /** Tuning fork icon. */
        Image icon;

    };
    
    //==========================================================================
    
    
    /**
     *  A block that is a setter for the tempo.
     */
    class PitchSetter    : public juckly::Block
    {
    public:
        
        /** Default constructor. */
        PitchSetter () : juckly::Block ( std::make_unique<juckly::CommandBlockSettings>
                                        ("PitchSetter",
                                         ImageCache::getFromMemory (BinaryData::pitchSetterBlockIcon_png,
                                                                    BinaryData::pitchSetterBlockIcon_pngSize),
                                         ImageCache::getFromMemory (BinaryData::lightBlueBlock_png,
                                                                    BinaryData::lightBlueBlock_pngSize),
                                         false /* not a start node */,
                                         new PitchSetterComponent(),
                                         false /* dosen't take a parameter */
                                         )
                                        )
        {
        }
        
        /** Changes the tempo to the slider value. */
        void doAction() override
        {
            // Change the tempo to the slider value.
            auto tuningOffset = dynamic_cast<PitchSetterComponent*>(getInternalUI());
            
            jassert (tuningOffset != nullptr); // Cast didn't work!
            if (tuningOffset != nullptr)
                PlaybackSettings::get().setTuningOffset (tuningOffset->tuningSlider.getValue());
        }
        
        //======================================================================
        
        /**
         *  Contains info for saving and loading the pitch setters internal data
         *  @param the head element for this block
         *  @param if save or load should be performed
         */
        void doSaveOrLoad (XmlElement* blockHead, FileManipulator mode) override
        {
            auto tuningOffset = dynamic_cast<PitchSetterComponent*>(getInternalUI());
            
            if (mode == FileManipulator::save)
            {
                blockHead->setAttribute ("tuningOffset",
                                         tuningOffset->tuningSlider.getValue());
            }
            
            //==================================================================
            
            if (mode == FileManipulator::load)
            {
                tuningOffset->tuningSlider.setValue (blockHead->getIntAttribute("tuningOffset"));
            }
        }
    private:
        
    };
    
} // namespace codetta
