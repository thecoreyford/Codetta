/*
  ==============================================================================

    DoubleQuaver.h
    Created: 3 Oct 2018 9:44:16pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "NoteFragment.h"
#include "../Note.h"

//==============================================================================

/** Namespace for the MusiSync Notation Engine */
namespace MusiSyncEng
{
    /**
     *  A DoubleQuaver Fragment
     */
    class DoubleQuaver    : public NoteFragment, public Note::Listener
    {
    public:
        /** Constructor */
        DoubleQuaver();
        
        /** Virtual destructor.*/
        ~DoubleQuaver();
        
        /**
         *  Draws beaming between notes
         *  @param the graphics context for this object
         */
        void paint (Graphics&) override;
        
        /** Sets bounds of child components. */
        void resized() override;
        
        /** Calls repaint. */
        void noteMoved() override;
        
        /**
         *  Gets the character representation for this fragment.
         *  @return the ascii representation char of this fragment value.
         */
        const char getChar() const override;
        
        /**
         *  Getter for the first notes within a fragment.
         *  @return pointer to the 1st note within a fragment.
         */
        const std::unique_ptr<Note>* getFirstNoteOfFragment() const override;
        
        /**
         *  Getter for the number of notes within a fragment.
         *  @return the number of notes within this fragment.
         */
        int getNoOfNotesInFragment() const override { return 2; }
        
        /**
         *  Getter for the note string value.
         *  @return the text/string representation for this note value.
         */
        const String getText() const override { return "double quaver"; }
        
    private:
        /** Array of notes in this fragment. */
        std::unique_ptr<Note> note[2];
        /** True if drawn for the first time. */
        bool firstTime = true;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DoubleQuaver)
    };
} // namespace MusiSyncEng

