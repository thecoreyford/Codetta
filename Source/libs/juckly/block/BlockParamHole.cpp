/*
  ==============================================================================

    BlockParamHole.cpp
    Created: 4 Sep 2018 6:48:42pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "BlockParamHole.h"
#include "BlockScaling.h"

/** Main namespace for the juckly library */
namespace juckly
{
    BlockParamHole::BlockParamHole()
    {
        internalParam  = nullptr;
    }
    
    BlockParamHole::~BlockParamHole()
    {
        if (internalParam == nullptr)
        {
            delete internalParam;
        }
        //else belongs to owned array
    }
    
    void BlockParamHole::addInternalParam (Component* newInternalParam)
    {
        if (newInternalParam != nullptr)
        {
            internalParam = newInternalParam;
            addAndMakeVisible (internalParam);
            resized();
        }
    }
    
    Component* BlockParamHole::getInternalParam()
    {
        return internalParam;
    }
    
    void BlockParamHole::paint (Graphics& g)
    {
        g.setColour (Colours::grey);
        g.fillRoundedRectangle (getLocalBounds().toFloat(), 5.0f);
    }
    
    void BlockParamHole::resized()
    {
        if (internalParam != nullptr)
        {
            Rectangle<int> bounds = getLocalBounds().expanded(20, 0);
            internalParam->setBounds (bounds);
            
            // Ensure clicking dosen't reset position.
            internalParam->setInterceptsMouseClicks(false, true);
        }
    }
    
} // namespace juckly
