/*
  ==============================================================================

    Note.h
    Created: 2 Oct 2018 9:04:04pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "Font.h"
#include "OffsetMappings.h"

//==============================================================================
#define CODETTA 0 //< add macro so that the libs folder can remain clean for
                  // potentially releasing as an API.
#ifdef CODETTA
    #include "../../gui/InfoBar.h"
#endif
//==============================================================================

/** Namespace for the MusiSync Notation Engine */
namespace MusiSyncEng
{
    /**
     *  Class representing a single note object.
     */
    class Note    : public Component
    {
    public:
        /**
         *  Constructor.
         *  @param This notes value.
         *  @param boolean if the note should invert past the central line.
         */
        Note (const NewFont::NoteValue valueRhs, bool flippableRhs = true);
        
        /** Destructor. */
        ~Note();
        
        /**
         *  Draws note position.
         *  @param the graphics context for this object
         */
        void paint (Graphics&) override;
        
        /** Sets bounds of up and down buttons. */
        void resized() override;
        
        /**
         *  Callback for the mouse move event deciding if buttons are visible.
         *  @param the mouse event itself.
         */
        void mouseMove (const MouseEvent& event) override;
        
        /**
         *  Getter for the length value of this note.
         *  @return a reference to the notes total value.
         */
        const NewFont::NoteValue& getValue() const;
        
        /**
         *  Getter for the notes bounds.
         *  @return the bounds for the note object.
         */
        Rectangle<int> getTextPosition() const;
        
        /** Listener for when the note changes position. */
        class Listener
        {
        public:
            /** Destructor */
            virtual ~Listener() {}
            
            /**
             *  Function altering listeners when the note has moved.
             *  Can be useful if redrawing for beaming.
             */
            virtual void noteMoved() = 0;
        };
        
        /**
         *  Setter for the listener object.
         *  @param the new listener.
         */
        void addListener (Listener* listenerRhs) { listener = listenerRhs; }
        
        /**
         *  Getter for the musical MIDI note value.
         *  @return note symbal for the notes pitch
         */
        NoteSym getMusicalValue()  { return mapping.getMapping()[pitchOffset]; }
        
        /**
         *  Setter for changing the mappings clef type.
         *  @param the new clef type.
         */
        void changeClef (ClefTypes clef) { mapping.setMappingToClef (clef); }
        
    private:
        
        /** Private destructor. */
        Note();

        /** This notes value. */
        const NewFont::NoteValue value;
        
        /** Up button object. */
        TextButton up;
        /** Down button object */
        TextButton down;
        
        /** The pitch offset of this note - starts at -1 */
        int pitchOffset = -1;
        
        /** Amount of increment visible per offset.*/
        int offsetIncrementAmount;
        
        /** The text position bounds.*/
        Rectangle<int> textPosition;
        
        /** Pointer for listener objects. */
        Listener* listener;
        
        /** True if note inverts past the centre line. */
        bool flippable = true;
        
        /** Mapping for the offset to its clef.*/
        OffsetMappings mapping;
    };
} // namespace MusicSyncEng
