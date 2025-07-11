/*
  ==============================================================================

    NoteFragmentPicker.h
    Created: 12 Oct 2018 12:39:13pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../Font.h"
#include "FragmentFactory.h"

//==============================================================================

#define CODETTA 0 //< add macro so that the libs folder can remain clean for
                  // potentially releasing as an API.
#ifdef CODETTA
    #include "../../../gui/InfoBar.h"
#endif

//==============================================================================

/** Namespace for the MusiSync Notation Engine */
namespace MusiSyncEng
{
    /**
     *  Drop down for selecting note fragments.
     */
    class NoteFragmentPicker    : public Component
    {
    public:
        /** Constructor */
        NoteFragmentPicker();
        
        /** Destructor */
        ~NoteFragmentPicker();
        
        /** Sets bounds of child components. */
        void resized() override;
        
        /** Listener for when a fragment is selected. */
        class Listener
        {
        public:
            /** Destructor. */
            virtual ~Listener() {}
            /**
             *  Called whenever a fragment has been chosen.
             *  @param the chosen fragment.
             */
            virtual void onFragmentSelected (Fragment noteValue) = 0;
        };
        
        /** Setter for listener classes. */
        void addListener (Listener* listenerRhs);
        
        /**
         *  Resets the menu to only show applicable notes.
         *  @param the threshold value for the fragments left in the menu.
         */
        void updateMenu (float valueLeft);
        
    private:
        /** Button to show pop up menu. */
        TextButton plus;
        
        //======================================================================
        /** Menu containing options. */
        PopupMenu menu;
        
        /** Look and feel class for our popup-menu in order to change font.  */
        class PopupMenuLookAndFeel    : public LookAndFeel_V4
        {
        public:
            /**
             *  Getter for our menus font.
             *  @return the MusiSync font.
             */
            Font getPopupMenuFont() override
            {
                return NewFont::getNotationFont().withHeight (26.f);
            }
        };
        
        /** Look and feel for our popup menu. */
        PopupMenuLookAndFeel menuLookAndFeel;
        //======================================================================
        
        /** Listeners to this popup menu. */
        Listener* listener;
        
    };
} // namespace MusicSyncEng
