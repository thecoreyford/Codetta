/*
  ==============================================================================

    DoubleQuaver.cpp
    Created: 3 Oct 2018 9:44:16pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "DoubleQuaver.h"

/** Namespace for the MusiSync Notation Engine */
namespace MusiSyncEng
{
    DoubleQuaver::DoubleQuaver() : NoteFragment (0.25f)
    {
        for (int n = 0; n < 2; ++n)
        {
            note[n] = std::make_unique<Note>(NewFont::NoteValue::crotchet, false);
            addAndMakeVisible (*note[n]);
            note[n]->addListener (this);
        }
    }
    
    DoubleQuaver::~DoubleQuaver()
    {
    }
    
    void DoubleQuaver::paint (Graphics& g)
    {
        if (!firstTime)
        {
            g.setColour (Colours::black);
            
            auto leftPos = note[0]->getTextPosition();
            auto rightPos = note[1]->getTextPosition();
            
            int leftX, leftY, rightX, rightY;

            leftX = leftPos.getWidth() * 0.5 + 8;
            leftY = leftPos.getY() + 30;

            rightX = rightPos.getWidth() * 0.5 + (getWidth() * 0.5) + 8;
            rightY = rightPos.getY() + 30;
            
            g.drawLine (leftX, leftY, rightX, rightY, 3);
        }
        else { firstTime = false; }
    }
    
    void DoubleQuaver::resized()
    {
        auto noteBounds1 = getLocalBounds();
        auto noteBounds2 = getLocalBounds();
        noteBounds1.removeFromRight (getWidth() * 0.5);
        noteBounds2.removeFromLeft (getWidth() * 0.5);
        
        note[0]->setBounds (noteBounds1);
        note[1]->setBounds (noteBounds2);
    }
    
    void DoubleQuaver::noteMoved()
    {
        repaint();
    }

    const char DoubleQuaver::getChar() const
    {
        return 'n';
    }
    
    const std::unique_ptr<Note>* DoubleQuaver::getFirstNoteOfFragment() const
    {
        return &note[0];
    }
    
    
} // namespace MusiSyncEng
