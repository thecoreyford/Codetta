/*
  ==============================================================================

    Bar.h
    Created: 3 Oct 2018 1:47:42pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../data-logger/DataLogger.h"
#include "Fragments/NoteFragment.h"
#include "Fragments/NoteFragmentPicker.h"
#include "Font.h"

using FragmentPtr = std::unique_ptr<MusiSyncEng::NoteFragment>;

//==============================================================================
#define CODETTA 0 //< add macro so that the libs folder can remain clean for
                  // potentially releasing as an API.
#ifdef CODETTA
    #include "../../gui/widgets/InfoBar.h"
    #include "../../gui/widgets/ContextTracker.h"
    #include "../../gui/widgets/UndoWidget.h"
#endif
//==============================================================================

/** Namespace for the MusiSync Notation Engine */
namespace MusiSyncEng
{
    /**
     *  Bar container for many fragments.
     */
    class Bar    : public Component,
                   public NoteFragmentPicker::Listener
    {
    public:
        /**
         *  Constructor.
         *  @param numerator for this bars time signature.
         *  @param denominator for this bars time signature.
         */
        Bar (int timeSigNumerator, int timeSigDenominator, Component* parent = nullptr);
        
        /** Destructor.*/
        ~Bar();
        
        /**
         *  Getter to retreive this notes  bar object.
         *  @return the parent object.
         */
        Component* getParent() { return parent; }
        
        /**
         *  Draws bar lines.
         *  @param the graphics context for this object
         */
        void paint (Graphics&) override;
        
        /** Sets bounds of child components. */
        void resized() override;
        
        //======================================================================
        /**
         *  Callback for the @NotePicker listener.
         *  @param the fragment that has been selected.
         */
        void onFragmentSelected (Fragment noteValue) override;
        
        /** Called to remove a fragment from a bar. */
        void onMinusClicked() override;
        
        //======================================================================
        
        /**
         *  Getter for all the bars notes.
         *  @return all fragments within the bar
         */
        const std::vector<FragmentPtr>& getFragment() const
        {
            return notes;
        }
        
        /**
         *  Getter for all value left.
         *  @return the space left within the bar.
         */
        const float& getValueLeft () const { return valueLeft; }
        
        //======================================================================

        /**
         *  Repeats the previously added note.
         *  @return if adding the note was succesfull.
         */
        bool duplicateLastNote();
        
        /** Shows the plus note-picker pop-up menu.*/
        void triggerPlus();
        
    private:
        
        /** Private constructor. Must state time signature. */
        Bar();
        
        /**
         *  Function to draw beaming ontop of notes!
         *  @param graphics context from @see paint
         */
        void drawBeaming(Graphics& g);
        
        /** Object for selecting notes to add to the bar.*/
        NoteFragmentPicker notePicker;
        
        /** Container for fragments present on the bar. */
        std::vector<FragmentPtr> notes;
        
        /** Maximum value the bar holds. */
        float maximumValue;
        
        /** Amount of space left in the bar. */
        float valueLeft;
        
        /** Parent class for the bar.*/
        Component* parent;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Bar)
    };

} // namespace MusiSyncEng

