/*
  ==============================================================================

    BasicNoteFragments.h
    Created: 5 Oct 2018 7:26:07pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "NoteFragment.h"
#include "../Font.h"
#include "../Note.h"

/** Namespace for the MusiSync Notation Engine */
namespace MusiSyncEng
{
    /**
     *  An abstraction class of NoteFragment for basic notes.
     */
    class BasicFragment    : public NoteFragment
    {
    public:
        /**
         *  Constructor.
         *  @param the notes value in length e.g. crotchet.
         */
        BasicFragment (NewFont::NoteValue noteValueRhs)
            : NoteFragment (NewFont::getNoteDivision (noteValueRhs)),
              noteValue (noteValueRhs)
        {
            note = std::make_unique<Note> (noteValue);
            addAndMakeVisible (*note);
        }
        
        /** Sets bounds for the note size. */
        void resized() override { note->setBounds (getLocalBounds()); }
        
        /**
         *  Getter for the note character.
         *  @return the ascii representation char of this note value.
         */
        const char getChar() const override
        {
            return NewFont::getNoteAsChar (noteValue);
        }
        
        /**
         *  Getter for the first notes within a fragment.
         *  @return pointer to the 1st note within a fragment.
         */
        const std::unique_ptr<Note>* getFirstNoteOfFragment() const override
        {
            return &note;
        }
        
        /**
         *  Getter for the note string value.
         *  @return the text/string representation for this note value.
         */
        virtual const String getText() const override
        {
            return NewFont::getNoteAsText (noteValue);
        }
        
        
        /**
         *  Getter for the number of notes within a fragment.
         *  @return the number of notes within this fragment.
         */
        int getNoOfNotesInFragment() const override { return 1; }
        
    private:
        
        /** Private constructor. Note value must be provided. */
        BasicFragment();
        
        /** Pointer to the note object contained within this fragment. */
        std::unique_ptr<Note> note;
        
        /** The note value for this fragment. */
        NewFont::NoteValue noteValue;
    };
    
    //==========================================================================
    
    /** Semibreve representation abstracted ontop of BasicFragment. */
    class Semibreve    : public BasicFragment
    {
    public:
        /** Constructor */
        Semibreve()    : BasicFragment (NewFont::NoteValue::semibreve) { }
    };

    //==========================================================================
    
    /** Minim representation abstracted ontop of BasicFragment. */
    class Minim    : public BasicFragment
    {
    public:
        /** Constructor */
        Minim()    : BasicFragment (NewFont::NoteValue::minim) { }
    };

    //==========================================================================
    
    /** Crotchet representation abstracted ontop of BasicFragment. */
    class Crotchet   : public BasicFragment
    {
    public:
        /** Constructor */
        Crotchet()    : BasicFragment (NewFont::NoteValue::crotchet) { }
    };
    
    //==========================================================================
    
    /** Quaver representation abstracted ontop of BasicFragment. */
    class Quaver   : public BasicFragment
    {
    public:
        /** Constructor */
        Quaver()    : BasicFragment (NewFont::NoteValue::quaver) { }
    };
    
    //==========================================================================
    
//    class Semiquaver   : public BasicFragment
//    {
//    public:
//        Semiquaver()    : BasicFragment (NewFont::NoteValue::semiquaver) { }
//    };
    
    //==========================================================================
    
} // namespace MusiSyncEng
