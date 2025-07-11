/*
 ==============================================================================
 
 NoteFragment.h
 Created: 3 Oct 2018 9:35:25pm
 Author:  Corey Ford
 
 ==============================================================================
 */

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../Note.h"

//==============================================================================

/** Namespace for the MusiSync Notation Engine */
namespace MusiSyncEng
{
    /**
     *  Pure virtual class representing a note fragment.
     */
    class NoteFragment    : public Component
    {
    public:
        /**
         *  Constructor.
         *  @param the total length of the fragment.
         */
        NoteFragment (const float totalDivisionRhs)
            : totalDivison (totalDivisionRhs) { }
        
        /** Destructor.*/
        virtual ~NoteFragment() {}
        
        /**
         *  Draws notes.
         *  @param the graphics context for this object
         */
        void paint (Graphics& g) override {}
        
        /** Sets bounds of child components. */
        void resized() override {}
        
        /**
         *  Getter for the length value of this fragment.
         *  @return a reference to the fragments total value.
         */
        const float& getValue() const { return totalDivison; }
        
        /**
         *  Getter for the note character.
         *  @return the ascii representation char of this note value.
         */
        virtual const char getChar() const = 0;
        
        /**
         *  Getter for the note string value.
         *  @return the text/string representation for this note value.
         */
        virtual const String getText() const = 0;
        
        /**
         *  Getter for the first notes within a fragment.
         *  @return pointer to the 1st note within a fragment.
         */
        virtual const std::unique_ptr<Note>* getFirstNoteOfFragment() const = 0;
        
        /**
         *  Getter for the number of notes within a fragment.
         *  @return the number of notes within this fragment.
         */
        virtual int getNoOfNotesInFragment() const = 0;
        
    protected:
        /** The total length of this fragment's value. */
        const float totalDivison;
        
    private:
        /** Private constructor. Total length of fragment must be specified */
        NoteFragment();
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteFragment)
    };
    
} // namespace MusiSyncEng

