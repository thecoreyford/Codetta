/*
  ==============================================================================

    Bar.cpp
    Created: 3 Oct 2018 1:47:42pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "Bar.h"

//==============================================================================

/** Namespace for the MusiSync Notation Engine */
namespace MusiSyncEng
{
    Bar::Bar (int timeSigNumerator, int timeSigDenominator)
    {
        addAndMakeVisible (notePicker);
        maximumValue = (float)timeSigNumerator / (float)timeSigDenominator;
        
        notePicker.addListener (this);
    }
    
    Bar::~Bar()
    {
    }
    
    void Bar::paint (Graphics& g)
    {
        g.fillAll (Colours::white);
        
        g.setColour (Colours::black);
        
        const float thickness = getHeight() / 12.0f;
        
        for (int i = 1; i <= 10; ++i)
        {
            if( i % 2 == 0 )
                g.drawLine (0,
                            i * thickness + (thickness * 0.25),
                            getWidth(),
                            i * thickness + (thickness * 0.25),
                            thickness * 0.5);
        }
    }
    
    void Bar::resized()
    {
        valueLeft = maximumValue;
        int x = 0;
        for (int i = 0; i < notes.size(); ++i)
        {
            auto thisNotesBounds =  getLocalBounds().toFloat();
            thisNotesBounds.setWidth (getLocalBounds().getWidth() * (notes[i]->getValue() / maximumValue));
            thisNotesBounds.translate (x, 0);
            notes[i]->setBounds (thisNotesBounds.toNearestInt());
            
            x += thisNotesBounds.getWidth();
            if (valueLeft - notes[i]->getValue() >= 0.f)
                valueLeft -= notes[i]->getValue();
            // else bar is full. 
        }
        
        // setup note picker
        auto smallestNoteValue = (NewFont::NoteValue)((int)NewFont::NoteValue::totalNoteValue - 1);
        
        auto notePickerBounds = getLocalBounds().toFloat();
        notePickerBounds.setWidth (getLocalBounds().getWidth()
                                  * (NewFont::getNoteDivision (smallestNoteValue) / maximumValue) );
        if (valueLeft <= 0.f)
        {
            // NO MORE NOTE PICKER
            notePicker.setVisible (false);
        }
        else
        {
            notePickerBounds.translate (x, 0);
            notePicker.setBounds (notePickerBounds.toNearestInt());
        }
        
        notePicker.updateMenu (valueLeft);
        
        repaint();
    }
    
    void Bar::onFragmentSelected (Fragment noteValue)
    {
        FragmentFactory factory;
        
        if (factory.createNoteFragment(noteValue) != nullptr) // user clicked off menu
        {
            notes.push_back (factory.createNoteFragment(noteValue));
            #ifdef CODETTA
                codetta::InfoBar::get().updateContents ("Added a "
                                                        + notes.back()->getText()
                                                        + " note!");
            #endif
            addAndMakeVisible (*notes.back());
            resized();
        }
    }

} // namespace MusiSyncEng
