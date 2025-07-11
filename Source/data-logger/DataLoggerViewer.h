/*
  ==============================================================================

    DataLoggerViewer.h
    Created: 12 Apr 2021 3:42:08pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../libs/MusicSync-Notation-Engine/Font.h"
#include "../libs/MusicSync-Notation-Engine/OffsetMappings.h"

/** Widget displaying text sent to it: used to notate the log values in real-time. */
class DataLoggerViewer    : public Component
{
public:
    
    /**
     *  Acessor function for the DataLoggerViewer singleton.
     *  @return reference to the DataLoggerViewer class.
     */
    static DataLoggerViewer& get()
    {
        static DataLoggerViewer instance;
        return instance;
    }
    
    /**
     *  Paint callback.
     *  @param the graphics context for this object.
     */
    void paint (Graphics& g) override
    {
        // draw background
        g.setColour (Colours::white);
        g.fillRoundedRectangle (getLocalBounds().toFloat(), 5);
        
        // add text
        g.setColour (Colours::black);
        g.setFont (getHeight() * 0.8);
        g.drawText (displayedMessage, getLocalBounds(), Justification::centred);
    }

    /**
     *  Updates the visible message.
     *  @param the new message to be written.
     */
    void update (const String& message)
    {
        displayedMessage = message;
        repaint();
    }
    
private:
    
    //======================================================================
    
    /** Private Constructor. */
    DataLoggerViewer() { displayedMessage = "0000"; }
    
    /** Private Destructor */
    ~DataLoggerViewer() {}

    /**
     *  Private Copy Constructor
     *  @param copyable value
     */
    DataLoggerViewer (const DataLoggerViewer&){}
    
    /**
     *  Private Copy Operator
     *  @param copyable value
     */
    void operator= (const DataLoggerViewer&){}
    
    //======================================================================
    
    /** The message curretly visible.*/
    String displayedMessage;
};
