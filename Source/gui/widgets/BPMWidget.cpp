/*
  ==============================================================================

    BPMWidget.cpp
    Created: 20 Jan 2019 12:30:56pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../data-logger/DataLogger.h"
#include "../../gui/widgets/UndoWidget.h"
#include "BPMWidget.h"

/** Main namespace for codetta. */
namespace codetta
{
    BPMWidget::BPMWidget()
    {
        globe = ImageCache::getFromMemory (BinaryData::globe_png,
                                           BinaryData::globe_pngSize);
        
        addAndMakeVisible (tempoSlider);
        tempoSlider.setColour (Slider::ColourIds::textBoxTextColourId,
                               Colours::black);
        tempoSlider.setRange (24.0, 550.0, 1.0);
        tempoSlider.setValue (120.0);
        
        tempoSlider.onValueChange = [this]
        {
            #ifdef USE_LOGGING
            if (PlaybackSettings::get().getGlobalTempo() >= tempoSlider.getValue())
                LOG_STRING("global tempo decreased");
            else
                LOG_STRING("global tempo increased");
            #endif
            
            PlaybackSettings::get().setGlobalTempo (tempoSlider.getValue());
            
            // Update the undo stack
//            codetta::UndoWidget::get().updateUndoStack();
        };
    }
    
    BPMWidget::~BPMWidget()
    {
       
    }
    
    void BPMWidget::paint (Graphics& g)
    {
        auto all = getLocalBounds().reduced (2, 2);
        
        g.setColour(Colour::fromRGB (161, 222, 209));
        g.fillRoundedRectangle (getLocalBounds().toFloat(), 5.5);
        g.setColour (Colours::black);
        g.drawRoundedRectangle (getLocalBounds().toFloat(), 5.5, 0.1);
        
        Rectangle<float> globeRect;
        globeRect.setWidth (getHeight());
        globeRect.setHeight (getHeight());
        g.drawImage (globe, globeRect);
        
        g.setFont (MusiSyncEng::NewFont::getNotationFont().withHeight (getHeight()));
        
        g.drawText ("q = ",
                    all.withTrimmedRight(getWidth() * 0.80).translated (globeRect.getWidth(), 0),
                    Justification::centredRight);
        
        tempoSlider.setBounds (all.withTrimmedLeft (getWidth() * 0.31));
    }
    
    void BPMWidget::resized()
    {
        // repaint();
    }

    const int BPMWidget::getTempo() const
    {
        return tempoSlider.getValue();
    }

    void BPMWidget::setTempo (const int newTempo)
    {
        tempoSlider.setValue ((double)newTempo);
    }



} // namespace codetta
