/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../libs/juckly/client/Workspace.h"
#include "../libs/juckly/client/Toolbox.h"
//#include "../libs/juckly/client/BlockFactory.h"
//#include "../libs/juckly/block/DemoBlocks.h"

#include "../blocks/CodettaBlockFactory.h"
#include "../blocks/Blocks.h"

#include "../audio/Audio.h"

// GUI
#include "PlayBackControls.h"
#include "CodettaViewport.h"
#include "InfoBar.h"
#include "BPMWidget.h"

/** Main namespace for codetta. */
namespace codetta
{
    /*
     This component lives inside our window, and this is where you should put all
     your controls and content.
     */
    class MainComponent   : public Component,
                            public DragAndDropContainer,
                            public MidiOut::Listener,
                            public PlayBackControls::Listener
    {
    public:
        //==============================================================================
        /**
         *  Constructor.
         *  @param reference to the audio class.
         */
        MainComponent (Audio& audioRhs);
        
        /** Destructor. */
        ~MainComponent();
        
        //==============================================================================
        /**
         *  Paint callback.
         *  @param the graphics context for this object.
         */
        void paint (Graphics&) override;
        
        /** Resized Callback. Sets bounds of child components. */
        void resized() override;
        
        //==============================================================================
        /** MidiOutput callback. */
        void onStoppedPlaying() override;
        
        //==============================================================================
        
        /**
         *  Callback for when the play button is clicked.
         *  @param reference to the play button.
         *  @param reference to the stop button.
         */
        void onPlayClicked (TextButton& play, TextButton& stop) override;
        
        /**
         *  Callback for when the stop button is clicked.
         *  @param reference to the play button.
         *  @param reference to the stop button.
         */
        void onStopClicked (TextButton& play, TextButton& stop) override;
        
        //=============================================================================
        
    private:
        
        /** Private Constructor. */
        MainComponent();
        
        Audio& audio;
        
        //==============================================================================
        // Gui Components
        /** The top layer toolbox component from juckly. */
        juckly::Toolbox toolbox;
        
        /** Play button. */
        PlayBackControls playbackControls;
        
        /** The view containing the top layer workspace component from juckly. */
        CodettaViewport viewport;
        
        /** Reference to the info bar singleton. */
        InfoBar& infoBar;
        
        /** Widget controlling the bpm. */
        BPMWidget bpmWidget;
        
        //==============================================================================
        // Stuff for tutorials  //TODO: abstract
        /** Web browser used to view tutorials.*/
        WebBrowserComponent browser;
        
        /** Button used to lead different tutorials.*/
        TextButton tutorialButton;
        
        /** Toggle for bounds for tutorials.*/
        bool tutIsShowing;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    };
} // namespace codetta
