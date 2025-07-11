/*
  ==============================================================================

    InfoBar.cpp
    Created: 11 Jan 2019 12:22:26pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "InfoBar.h"

/** Main namespace for codetta. */
namespace codetta
{
    void InfoBar::paint (Graphics& g)
    {
        // draw background
        g.setColour (Colours::darkgrey);
        g.fillRoundedRectangle (getLocalBounds().toFloat(), 5);
        
        // add text
        g.setColour (Colours::white);
        g.setFont/*height*/ (20.0);
        g.drawText (displayedMessage, getLocalBounds(), Justification::centred);
    }
    
    void InfoBar::updateContents()
    {
        displayedMessage = "Codetta";
        repaint();
    }
    
    void InfoBar::updateContents (const String& message)
    {
        displayedMessage = message;
        repaint();
    }
    
    void InfoBar::updateContentForNote (int offsetMapping,
                                        MusiSyncEng::NewFont::NoteValue value)
    {
        MusiSyncEng::OffsetMappings mapping;
        auto treblePitch = mapping.getNoteSymForAnyClef(MusiSyncEng::ClefTypes::Treble, offsetMapping);
        auto bassPitch = mapping.getNoteSymForAnyClef(MusiSyncEng::ClefTypes::Bass, offsetMapping);
        
        String noteValue = MusiSyncEng::NewFont::getNoteAsText (value);
        String trebleMIDI = MidiMessage::getMidiNoteName (treblePitch, true, true, 4);
        String bassMIDI = MidiMessage::getMidiNoteName (bassPitch, true, true, 4);
        
        displayedMessage = noteValue.toUpperCase()
                           + "     |     "
                           + "Treble = " + trebleMIDI
                           + "     |     "
                           + "Bass = " + bassMIDI;
        
        repaint();
    }
} // namespace codetta
