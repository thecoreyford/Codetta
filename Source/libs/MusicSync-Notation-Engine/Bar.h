/*
  ==============================================================================

    Bar.h
    Created: 3 Oct 2018 1:47:42pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "Fragments/NoteFragment.h"
#include "Fragments/NoteFragmentPicker.h"
#include "Font.h"

using FragmentPtr = std::unique_ptr<MusiSyncEng::NoteFragment>;

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
        Bar (int timeSigNumerator, int timeSigDenominator);
        
        /** Destructor.*/
        ~Bar();
        
        /**
         *  Draws bar lines.
         *  @param the graphics context for this object
         */
        void paint (Graphics&) override;
        
        /** Sets bounds of child components. */
        void resized() override;
        
        /**
         *  Callback for the @NotePicker listener.
         *  @param the fragment that has been selected.
         */
        void onFragmentSelected (Fragment noteValue) override;
        
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
        
    private:
        
        /** Private constructor. Must state time signature. */
        Bar();
        
        /** Object for selecting notes to add to the bar.*/
        NoteFragmentPicker notePicker;
        
        /** Container for fragments present on the bar. */
        std::vector<FragmentPtr> notes;
        
        /** Maximum value the bar holds. */
        float maximumValue;
        
        /** Amount of space left in the bar. */
        float valueLeft;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Bar)
    };

} // namespace MusiSyncEng

