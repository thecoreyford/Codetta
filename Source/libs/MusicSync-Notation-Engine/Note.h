/*
  ==============================================================================

    Note.h
    Created: 2 Oct 2018 9:04:04pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../data-logger/DataLogger.h"
#include "Font.h"
#include "OffsetMappings.h"

//==============================================================================
#define CODETTA 0 //< add macro so that the libs folder can remain clean for
                  // potentially releasing as an API.
#ifdef CODETTA
    #include "../../gui/widgets/InfoBar.h"
    #include "../../gui/widgets/ContextTracker.h"
    #include "../../gui/widgets/UndoWidget.h"
#endif
//==============================================================================
#ifdef JUCE_WINDOWS
#define M_PI 3.14159265358979323846264338327950288
#endif
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
        
        
        //======================================================================
        
        /**
         *  Getter for the pitch offset value.
         *  @return the pitch offset of this note.
         */
        const int& getPitchOffset() const { return pitchOffset; }
        
        /**
         *  Setter for pitch offset.
         *  @param the new pitch offset value.
         */
        void setPitchOffset (const int& newPitch);
        
        /**
         *  Setter for a notes colour.
         *  @param the new colour for the note.
         */
        void setCurrentColour (Colour newColour);
        
        /**
         *  Setter for the parent bar object.
         *  @param the new parent bar object.
         */
        void setParentBar (Component* newParentBar) { parentBar = newParentBar; }
        
        /**
         *  Getter to retreive this notes  bar object.
         *  @return the parent object.
         */
        Component* getParentBar() { return parentBar; }
        
        /**
         *  Getter for the offset increment amount.
         *  @return the offset increment amount.
         */
        const int& getOffsetIncrementAmount() const { return offsetIncrementAmount; }
        
        //TODO: setter and getter me!
        /** If the note is beamed represent a single stemed note. */
        bool beamed; 
        
        //======================================================================
        
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
        
        /** Pointer to the bar container. */
        Component* parentBar;
        
        /** True if note inverts past the centre line. */
        bool flippable = true;
        
        /** Mapping for the offset to its clef.*/
        OffsetMappings mapping;
        
        /** Colour for the note* */
        Colour currentColour;
    };
} // namespace MusicSyncEng
