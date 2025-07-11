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
    Bar::Bar (int timeSigNumerator, int timeSigDenominator, Component* parentRhs)
        : parent (parentRhs)
    {
        addAndMakeVisible (notePicker);
        maximumValue = (float)timeSigNumerator / (float)timeSigDenominator;
        
        notePicker.setAlwaysOnTop (true); //< for minus notes
        notePicker.addListener (this);
        
        codetta::ContextTracker::get().updateLastUsed ("Bar", this);
    }
    
    Bar::~Bar()
    {
        parent = nullptr;
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
        
        drawBeaming(g); // ontop of notes!
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
            // NO MORE NOTE addition
            notePicker.setBounds (getLocalBounds().withTrimmedLeft(getWidth() * 0.95));
            notePicker.setPlusEnabled (false);
//            notePicker.setVisible (false);
        }
        else
        {
            notePicker.setPlusEnabled();
            notePickerBounds.translate (x, 0);
            notePicker.setBounds (notePickerBounds.toNearestInt());
        }
        
        notePicker.updateMenu (valueLeft);
        
        repaint();
    }
    
    //==========================================================================
    
    void Bar::onFragmentSelected (Fragment noteValue)
    {
        FragmentFactory factory;
        
        #ifdef CODETTA
        codetta::ContextTracker::get().updateLastUsed ("Bar", this);
        #endif

        if (factory.createNoteFragment (noteValue) != nullptr) // user clicked off menu
        {
            //get the previous note's pitch value
            int oldPitch = -1;
            if (notes.size() >= 1)
                oldPitch = notes.back().get()->getFirstNoteOfFragment()->get()->getPitchOffset();

            // create the note using the factory
            notes.push_back (factory.createNoteFragment(noteValue));
            //assign the parent to this bar
            notes.back().get()->getFirstNoteOfFragment()->get()->setParentBar (this);
            #ifdef CODETTA
                codetta::InfoBar::get().updateContents ("Added a "
                                                        + notes.back()->getText()
                                                        + " note!");
            
                codetta::UndoWidget::get().updateUndoStack();
            #endif
            LOG_STRING("Added a " + notes.back()->getText() + " note");
            
            // Set pitch to the same as the previous note (Thanks MAX!!!)
            if (notes.size() >= 1)
                notes.back().get()->getFirstNoteOfFragment()->get()->setPitchOffset(oldPitch);
        
            addAndMakeVisible (*notes.back());
        }
        resized();
    }
    
    void Bar::onMinusClicked()
    {
        if (notes.size() > 0)
        {
            notes.pop_back();
            
            #ifdef CODETTA
            codetta::ContextTracker::get().updateLastUsed("Bar", this);
            codetta::InfoBar::get().updateContents ("Note removed!");
            codetta::UndoWidget::get().updateUndoStack();
            #endif
            
            LOG_STRING("Removed a note");
        }
        resized();
    }

    void Bar::drawBeaming (Graphics& g)
    {
        // set all notes to not beamed by default
        for (int i = 0; i < notes.size(); i++)
        {
            auto n = notes.at(i).get()->getFirstNoteOfFragment()->get();
            n->beamed = false;
        }
        
        //____________________________//
        
        if (notes.size() > 1)
        {
            for (int i = 1; i < notes.size(); i++)
            {
                auto& current = notes.at (i);
                auto& prev = notes.at (i-1);
                
                if (current->getText() == "quaver" && prev->getText() == "quaver")
                {
                    // set notes to be beamed
                    prev->getFirstNoteOfFragment()->get()->beamed = true;
                    current->getFirstNoteOfFragment()->get()->beamed = true;
                    
                    // get info from the notes
                    auto prevPitchOffset = prev->getFirstNoteOfFragment()->get()->getPitchOffset();
                    auto currentPitchOffset = current->getFirstNoteOfFragment()->get()->getPitchOffset();
                    auto offsetIncrementAmount = prev->getFirstNoteOfFragment()->get()->getOffsetIncrementAmount();
                    
                    // calculate translation amounts
                    auto prevTrans = prevPitchOffset * offsetIncrementAmount;
                    auto currentTrans = currentPitchOffset * offsetIncrementAmount;
                    
                    // figure out beam points
                    int prevX, prevY, currX, currY;
                    if (prevPitchOffset >= 5 /* we are above the middle of the stave*/)
                    {
                        prevY = prev.get()->getY() + 95 - prevTrans;
                        prevX = prev.get()->getX() + 7;
                    }
                    else
                    {
                        prevY = prev.get()->getY() + 24 - prevTrans;
                        prevX = prev.get()->getX() + 21;
                    }
                    
                    if (currentPitchOffset >= 5 /* we are above the middle of the stave*/)
                    {
                        currY = current.get()->getY() + 95 - currentTrans;
                        currX = current.get()->getX() + 10;
                    }
                    else
                    {
                        currY = current.get()->getY() + 24 - currentTrans;
                        currX = current.get()->getX() + 22;
                    }
                    
                    // draw onto bar
                    g.setColour(Colours::black);
                    g.drawLine (prevX,
                                prevY,
                                currX,
                                currY,
                                5.0);
                    
                    // skip the next note for beaming
                    i = i + 1;
                }
            }
        }
        
        // have to redraw, updating the parent object!
        if (parent != nullptr)
            parent->repaint();
    }

    //======

    bool Bar::duplicateLastNote()
    {
        if (notes.size() > 0){
            auto n = notes.back()->getValue();
            if (n == 1 && n <= valueLeft) {onFragmentSelected(Fragment::semibreve); return true;}
            if (n == 0.5 && n <= valueLeft) {onFragmentSelected(Fragment::minim); return true;}
            if (n == 0.25 && n <= valueLeft) {onFragmentSelected(Fragment::crotchet); return true;}
            if (n == 0.125 && n <= valueLeft) {onFragmentSelected(Fragment::quaver); return true;}
        }
        
        if (valueLeft == 0.0)
        {
            return false;
        }
        
        return true;
    }

    void Bar::triggerPlus()
    {
        notePicker.showPlus();
    }
} // namespace MusiSyncEng
