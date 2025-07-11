/*
 ==============================================================================
 
    Note.cpp
    Created: 2 Oct 2018 9:04:04pm
    Author:  Corey Ford
 
 ==============================================================================
 */

#include "Note.h"

/** Namespace for the MusiSync Notation Engine */
namespace MusiSyncEng
{
    Note::Note (NewFont::NoteValue valueRhs, bool flippableRhs)
    : value (valueRhs), flippable (flippableRhs)
    {
        // draw black notes by default
        currentColour = Colours::black;
        
        beamed = false;
        
        listener = nullptr;
        
        addAndMakeVisible (up);
        up.setVisible (false); // by default
        up.setButtonText ("/\\");
        up.setAlpha (0.5);
        up.onClick = [this]
        {
            if (listener != nullptr)
                listener->noteMoved();
            if (pitchOffset < 9)
                pitchOffset++;
            repaint();
            
        #ifdef CODETTA
            codetta::ContextTracker::get().updateLastUsed ("Note", this);
            codetta::ContextTracker::get().updateLastUsed ("Bar", parentBar);
            codetta::InfoBar::get().updateContentForNote (pitchOffset, value);
            codetta::UndoWidget::get().updateUndoStack();
        #endif
            LOG_STRING("Note moved up");
        };
        
        addAndMakeVisible (down);
        down.setVisible (false);
        down.setButtonText("\\/");
        down.setAlpha (0.5);
        down.onClick = [this]
        {
            if (listener != nullptr)
                listener->noteMoved();
            if (pitchOffset > -3)
                pitchOffset--;
            repaint();
            
        #ifdef CODETTA
            codetta::ContextTracker::get().updateLastUsed ("Note", this);
            codetta::ContextTracker::get().updateLastUsed ("Bar", parentBar);
            codetta::InfoBar::get().updateContentForNote (pitchOffset, value);
            codetta::UndoWidget::get().updateUndoStack();
        #endif
            LOG_STRING("Note moved down");
        };
        
        resized();
    }
    
    Note::~Note()
    {
        if (listener == nullptr)
            delete listener;
    }
    
    void Note::mouseMove (const MouseEvent& e)
    {
        if (up.getBounds().contains (e.getPosition()) || down.getBounds().contains (e.getPosition()))
        {
            up.setVisible (true);
            down.setVisible (true);
        }
        else
        {
            up.setVisible (false);
            down.setVisible (false);
        }
    }
    
    void Note::paint (Graphics& g)
    {
        textPosition = getLocalBounds();
        textPosition.removeFromBottom (4);
        bool flipped = false;
        
        if (flippable)
        {
            auto translationAmount = pitchOffset * offsetIncrementAmount;
            const auto transformedPosition = 3 * offsetIncrementAmount;
            if ((pitchOffset >= 5 && value != NewFont::NoteValue::quaver) ||
                (pitchOffset >= 5 && beamed == true) ) // were above middle of the stave
            {
                flipped = true;
                auto centreX = textPosition.getWidth() * 0.5f;
                auto centreY = textPosition.getHeight() * 0.65f;
                g.addTransform (AffineTransform::rotation (M_PI, centreX, centreY));
                textPosition.translate (10, -(transformedPosition - translationAmount));
            }
            else
            {
                flipped = false;
                auto centreX = getWidth() * 0.5f;
                auto centreY = getHeight() * 0.5f;
                g.addTransform (AffineTransform::rotation (0, centreX, centreY));
                textPosition.translate (10, -(translationAmount) + 2);
            }
        }
        else
        {
            auto translationAmount = pitchOffset * offsetIncrementAmount;
            auto centreX = getWidth() * 0.5f;
            auto centreY = getHeight() * 0.5f;
            g.addTransform (AffineTransform::rotation (0, centreX, centreY));
            textPosition.translate (10, -(translationAmount));
        }
        
        //===============================================================================
        // Draw ledger lines.
        if (pitchOffset == -3)
            g.drawLine (textPosition.getX() - 3, //+ (textPosition.getWidth() * 0.2) - 5,
                        textPosition.getY() * 3.9,
                        textPosition.getWidth() - (textPosition.getWidth() * 0.2),// - (textPosition.getWidth() * 0.2) - 5,
                        textPosition.getY() * 3.9,
                        3.0);
        
        if (pitchOffset == 9)
            g.drawLine (textPosition.getX() + 3,//(textPosition.getWidth() * 0.2) ,//- 5,
                        textPosition.getHeight() + 17,
                        textPosition.getWidth() + (textPosition.getWidth() * 0.2),// - 5,
                        textPosition.getHeight() + 17,
                        3.0);
        //===============================================================================
        
        g.setFont (NewFont::getNotationFont().withHeight (textPosition.getHeight()));
        g.setColour (currentColour);
        
        //================================================================================
        // Draw the correct quaver character if beamed
        String text;
        if (beamed == true) {
            text = String::charToString(NewFont::getNoteAsChar (NewFont::NoteValue::crotchet));
        } else {
            text = String::charToString(NewFont::getNoteAsChar (value));
        }
        //================================================================================
        
        if (value == NewFont::NoteValue::quaver && pitchOffset >= 5 && beamed == false)
        {
            // If true then the tail of the quaver should be inverted...
            // so we will use an image!
            Image quaver = ImageCache::getFromMemory (BinaryData::upsideDownQuaver_png,
                                                      BinaryData::upsideDownQuaver_pngSize);
            
            // ...and fiddle with it until it looks correct!
            auto pos = textPosition.reduced(6).withTrimmedTop(25).toFloat();
            pos.translate (-10.5, 20);
            pos.removeFromLeft (3.25);
            pos.removeFromRight (3.25);
            g.drawImage (quaver, pos);
        }
        else
        {
            // else we can just use the font...
            textPosition.removeFromLeft (0.3);
            g.drawText (text,
                        textPosition,
                        flipped ? Justification::centredRight : Justification::centredLeft,
                        true);
        }
    }
    
    void Note::resized()
    {
        offsetIncrementAmount = getHeight() / 12.0f;
        
        auto upBounds = getLocalBounds();
        auto downBounds = getLocalBounds();
        
        upBounds.removeFromBottom (getLocalBounds().getHeight() * 0.5);
        downBounds.removeFromTop (getLocalBounds().getHeight() * 0.5);
        
        upBounds.reduce (5, 5);
        downBounds.reduce (5, 5);
        
        up.setBounds (upBounds);
        down.setBounds (downBounds);
    }
    
    const NewFont::NoteValue& Note::getValue() const
    {
        return value;
    }
    
    Rectangle<int> Note::getTextPosition() const
    {
        return textPosition;
    }
    
    void Note::setPitchOffset (const int& newPitch)
    {
        if (newPitch <= 9 && newPitch >= -3){
            pitchOffset = newPitch;
            resized(); repaint();
        }
        #ifdef CODETTA
        codetta::ContextTracker::get().updateLastUsed ("Note", this);
        #endif
    }

    void Note::setCurrentColour (Colour newColour)
    {
        currentColour = newColour;
        repaint();
    }

    
} // namespace MusiSyncEng
