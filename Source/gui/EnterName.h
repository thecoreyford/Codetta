/*
  ==============================================================================

    EnterName.h
    Created: 29 Feb 2020 12:45:14pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

/** Main namespace for codetta. */ 
namespace codetta
{
    /**
     *  Custom GUI for enter name box.
     */
    class EnterName    : public Component
    {
    public:
        /** Constructor. */
        EnterName()
        {
            setSize (300, 100);
            
            username.setFont (Font (27.0f, Font::bold));
            addAndMakeVisible (username);
            username.onTextChange = [this]()
            {
                if (username.getText().contains(" ")){
                    go.setEnabled(false);
                }
                else if (username.isEmpty()){
                    go.setEnabled(false);
                }
                else{
                    go.setEnabled(true);
                }
            };
            
            go.setButtonText ("Go!");
            go.setColour (TextButton::buttonColourId, Colours::green);
            go.setColour (TextButton::textColourOnId, Colours::black);
            go.setEnabled(false);
            addAndMakeVisible (go);
            go.onClick = [this]()
            {
                DialogWindow* dw = this->findParentComponentOfClass<DialogWindow>();
                if (dw != nullptr)
                    dw->exitModalState (1234);
            };
        }
        
        /** Destructor */
        ~EnterName()
        {
            
        }
        
        /**
         *  Paint callback.
         *  @param graphics context.
         */
        void paint (Graphics& g) override
        {
            
        }
        
        /** Resized function - sets bounds. */
        void resized() override
        {
            auto bounds = getLocalBounds();
            username.setBounds (bounds.withTrimmedBottom(getHeight() * 0.30));
            go.setBounds (bounds.withTrimmedTop(getHeight() * 0.70));
        }
        
        /**
         *  Getter for the text edit's text.
         *  @return the text inside of the username text edit.
         */
        const String getText() const
        {
            return username.getText();
        }
        
    private:
        /** Text box for typing your name. */
        TextEditor username;
        /** Go button for submitting your name! */
        TextButton go;
    };

} // namespace codetta
