/*
  ==============================================================================

    BlockGUI.cpp
    Created: 30 Aug 2018 6:34:07pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "BlockGUI.h"

/** Main namespace for the juckly library */
namespace juckly
{
    BlockGUI::BlockGUI (Image toolboxIconRhs, Image workspaceImageRhs, BlockScaling& scalingRhs)
                        :   toolboxIcon (toolboxIconRhs),
                            workspaceImage (workspaceImageRhs),
                            scaling (scalingRhs)
    {
        // An image you have provided isn't valid. Ensure you have passed the
        // image correctly from the BinaryData using the JUCE ImageCache e.g.
        //     ImageCache::getFromMemory(BinaryData::block_png,
        //                               BinaryData::block_pngSize);
        if (!toolboxIcon.isValid() || !workspaceImage.isValid())
            jassert(false);
        
        overlapState = OverlapState::noOverlap;
    }
    
    const Image& BlockGUI::getToolboxIcon() const
    {
        return toolboxIcon;
    }
    
    void BlockGUI::paint (Graphics& g,
                          Rectangle<int> localBounds,
                          bool paintUI,
                          Rectangle<float>& internalUIBounds)
    {
        // draw component block image
        g.setOpacity (1.0f);
        g.drawImage (workspaceImage, localBounds.toFloat());
        
        // set hi-light areas
        Rectangle<float> visualIn, visualOut;
        visualIn.setSize (scaling.getConnectionWidth(), scaling.getHeight());
        
        visualOut.setSize (scaling.getConnectionWidth(),
                           scaling.getHeight());
        visualOut.translate (scaling.getWidth() - scaling.getConnectionWidth(), 0);
        
        
        Rectangle<float> param = localBounds.toFloat();
        param.removeFromLeft (scaling.getConnectionWidth()
                                         + scaling.getComponentBoarder() - 4.0f);
        param.removeFromRight (scaling.getConnectionWidth()
                                          + scaling.getComponentBoarder() - 4.0f);
        param.removeFromTop (scaling.getComponentBoarder() - 4.0f);
        param.removeFromBottom (scaling.getComponentBoarder() - 4.0f);
        
        // set hi-light colour
        g.setColour(Colours::yellow);
        g.setOpacity(0.25);
        
        // apply correct hi-lighting
        if (overlapState == OverlapState::inputOverlap)    g.fillRect (visualIn);
        if (overlapState == OverlapState::outputOverlap)    g.fillRect (visualOut);
        if(overlapState == OverlapState::paramOverlap)    g.fillRect (param);

        if(overlapState == OverlapState::inOutOverlap)
        {
            g.fillRect (visualIn);
            g.fillRect (visualOut);
        }
        
        // paint background for internalUI
        if (paintUI)
        {
            g.setColour (Colours::white);
            g.fillRoundedRectangle (internalUIBounds, 5.0f);
        }
    }
    
    void BlockGUI::setOverlapState(Component* block, OverlapState newOverlapState)
    {
        overlapState = newOverlapState;
        block->repaint();
    }
    
} // namespace juckly
