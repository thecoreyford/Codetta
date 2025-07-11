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
        plus.setConnectedEdges(Button::ConnectedOnBottom);
        minus.setConnectedEdges(Button::ConnectedOnTop);
        
        listener = nullptr;
        
        menu.setLookAndFeel (&menuLookAndFeel);
        
        updateMenu (1.0);
        
        setPlusEnabled();
        
        // Implement button features for adding notes
        addAndMakeVisible (plus);
        plus.setButtonText ("+");
        plus.setColour (TextButton::ColourIds::buttonColourId,
                        Colours::navy/*Colour::fromRGB (50, 158, 221)*/);
        plus.onClick = [this]
        {
            #ifdef CODETTA
                codetta::InfoBar::get().updateContents ("Add note!");
            #endif
            LOG_STRING("Note picker plus selected");
            
            menu.showMenuAsync(PopupMenu::Options(), [&](int result){
                if (listener != nullptr)
                    listener->onFragmentSelected ((Fragment)(result-1));
                           
               #ifdef CODETTA
               if (result == 0)
                   codetta::InfoBar::get().updateContents();
               #endif
            });
        };
        
        // Implement button features for removing notes
        addAndMakeVisible (minus);
        minus.setButtonText ("-");
        minus.setColour (TextButton::ColourIds::buttonColourId,
                         Colour::fromRGB (130, 186, 208));
        minus.onClick = [this]
        {
            LOG_STRING("Note picker minus selected");
            if (listener != nullptr)
                listener->onMinusClicked();
        };
        

    }
    
    NoteFragmentPicker::~NoteFragmentPicker()
    {
        menu.dismissAllActiveMenus();
        menu.setLookAndFeel (nullptr);
    }
    
    void NoteFragmentPicker::resized()
    {
        plus.setBounds (getLocalBounds().withTrimmedBottom(getHeight() * 0.40));
        minus.setBounds (getLocalBounds().withTrimmedTop (getHeight() * 0.60));
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
    
    void NoteFragmentPicker::setPlusEnabled (bool isEnabled)
    {
        if (!isEnabled)
        {
            plus.setEnabled (false);
            plus.setAlpha (1.0);
            minus.setAlpha (5.0f);
        }
        else
        {
            plus.setEnabled (true);
            plus.setAlpha (255);
            minus.setAlpha (255);
        }
    }

    void NoteFragmentPicker::showPlus()
    {
        plus.onClick();
    }
}
