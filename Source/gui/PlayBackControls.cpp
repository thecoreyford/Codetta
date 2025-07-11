/*
  ==============================================================================

    PlayBackControls.cpp
    Created: 8 Nov 2018 12:37:25pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "PlayBackControls.h"

//==============================================================================
/** Main namespace for codetta. */
namespace codetta
{
    PlayBackControls::PlayBackControls()
    {
        listener = nullptr;
        
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
        addAndMakeVisible (play);
        play.setButtonText ("Play");
        play.onClick = [this]
        {
            if(listener != nullptr)
                listener->onPlayClicked (play, stop);
        };
        play.setColour (TextButton::ColourIds::buttonColourId, Colours::lime);
        play.setColour (TextButton::ColourIds::textColourOffId, Colours::black);
        
        //======================================================================
        
        addAndMakeVisible (stop);
        stop.setButtonText ("Stop");
        stop.onClick = [this]
        {
            if(listener != nullptr)
                listener->onStopClicked (play, stop);
        };
        stop.setEnabled (false);
        stop.setColour (TextButton::ColourIds::buttonColourId, Colours::red);
    }
    
    PlayBackControls::~PlayBackControls(){}
    
    void PlayBackControls::paint (Graphics& g)
    {
        g.fillAll (Colours::black);
    }
    
    void PlayBackControls::resized()
    {
        auto all = getLocalBounds();
        
        auto top = all;
        top.removeFromBottom (getHeight() * 0.333333);
        
        auto bottom = all;
        bottom.removeFromTop (getHeight() * 0.666666);
        
        play.setBounds (top);
        stop.setBounds (bottom);
    }
    
    void PlayBackControls::addListener (Listener* newListener)
    {
        listener = newListener; 
    }
    
    void PlayBackControls::onPlayingStopped()
    {
        InfoBar::get().updateContents ("Music stopped!");
        play.setEnabled (true);
        play.setButtonText ("Play");
        stop.setEnabled (false);
    }
    //==========================================================================
    
} // namespace codetta

