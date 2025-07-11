/*
  ==============================================================================

    NoteFragmentPicker.cpp
    Created: 12 Oct 2018 12:39:13pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "NoteFragmentPicker.h"

/** Namespace for the MusiSync Notation Engine */
namespace MusiSyncEng
{
    NoteFragmentPicker::NoteFragmentPicker()
    {
        listener = nullptr;
        
        menu.setLookAndFeel (&menuLookAndFeel);
        
        updateMenu (1.0);
        
        // Implement button features & functionallity
        addAndMakeVisible (plus);
        plus.setButtonText ("+");
        plus.onClick = [this]
        {
            #ifdef CODETTA
                codetta::InfoBar::get().updateContents ("Note picker clicked!");
            #endif
            
            menu.showMenuAsync(juce::PopupMenu::Options(), [this](int result)
            {
                if (listener != nullptr && result > 0)
                    listener->onFragmentSelected(static_cast<Fragment>(result - 1));

                #ifdef CODETTA
                if (result == 0)
                    codetta::InfoBar::get().updateContents();
                #endif
            });

        };
    }
    
    NoteFragmentPicker::~NoteFragmentPicker()
    {
        menu.dismissAllActiveMenus();
        menu.setLookAndFeel (nullptr);
    }
    
    void NoteFragmentPicker::resized()
    {
        plus.setBounds (getLocalBounds());
    }
    
    void NoteFragmentPicker::addListener (Listener* listenerRhs)
    {
        listener = listenerRhs;
    }
    
    void NoteFragmentPicker::updateMenu (float valueLeft)
    {
        menu.clear();
        
        for (int i = 0; i < static_cast<int>(Fragment::totalFragments); ++i)
        {
            FragmentFactory factory;
            
            auto fragment = factory.createNoteFragment( (Fragment)i );
            
            // A fragment has not been found within the factory.
            jassert (fragment != nullptr);
            
            if (fragment->getValue() <= valueLeft)
            {
                auto itemSymbol = fragment->getChar();
                String addToList = String::charToString (itemSymbol);
                
                // add extra text to help distinquish small chars
                if (itemSymbol == 'w') addToList = "w = 1/1";
                if (itemSymbol == 'h') addToList = "h = 1/2";
                if (itemSymbol == 'q') addToList = "q = 1/4";
                if (itemSymbol == 'e') addToList = "e = 1/8";
                
                menu.addItem (i+1, addToList);
            }
        }
    }
}
