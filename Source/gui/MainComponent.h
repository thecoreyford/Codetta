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
#include "widgets/PlayBackControls.h"
#include "widgets/InfoBar.h"
#include "widgets/BPMWidget.h"
#include "widgets/UndoWidget.h"
#include "widgets/MenuBar.h"
#include "widgets/ContextTracker.h"
#include "LicenseInfo.h"
#include "LiveGuiPlayback.h"
#include "CodettaViewport.h"
#include "EnterName.h"


// Data Logging (see file for definition)
#include "../data-logger/DataLogger.h"
#include "../data-logger/DataLoggerViewer.h"

// Codetta Preprocessor-Definitions
#undef CODETTA_TRIAL
#define CODETTA_SHOW_TUTORIALS_FIRST
#define CODETTA_KEYBOARD_CONTROLS

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
                            public PlayBackControls::Listener,
                            public Button::Listener,
                            public MenuBar::Listener,
                            public Timer,
                            public UndoWidget::Listener,
                            public ContextTracker::Listener
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
        
        void loopOverBlocks();
        
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
        
        /**
         *  Callback for button listener (used for tutorial button).
         *  @param button triggering callback.
         */
        void buttonClicked (Button* button) override;
        
        //==============================================================================
        
        void timerCallback() override;
        
        void showAudioSettings();
        
        //==============================================================================
        
        void menuBarPressed (const String& item) override;

        void save();
        
        void writeSave(File* savedFile);
        
        XmlElement storeXmlElement();
        
        void load();
        
        void loadXmlElement(XmlElement xmlElement);
        
        void midiExport();
        
        XmlElement onUndoCommand(bool update, XmlElement elm) override;
        
        /** Calls message box to ensure the user has saved their work. */
        void areYouSureYouWantToQuitWithoutSaving();
        
        static void alertBoxResultChosen (int result, MainComponent* comp)
        {
            if (result == 1)
            {
                comp->save();
            }
            else
            {
                // Trigger quitting
                JUCEApplication::getInstance()->systemRequestedQuit();
            }
        }
        
        //==============================================================================
        #ifdef CODETTA_KEYBOARD_CONTROLS
        bool keyPressed (const KeyPress &k) override;
        #endif
        //TODO: comment
        void onContext (Component* comp) override;
        

    private:
        
        /** Private Constructor. */
        MainComponent();
        
        /** Reference to audio */
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
        // Stuff for tutorials
        /** Web browser used to view tutorials.*/
        WebBrowserComponent browser; //<web?
        
        /** Button used to lead different tutorials.*/
        TextButton tutorialButton;
        
        /** Toggle for bounds for tutorials.*/
        bool tutIsShowing;
        
        /** Button used to close the tutorial pane. */
        TextButton browserExitButton;
        
//        #ifdef CODETTA_TRIAL
        /** Counter for time spent using the application */
        int timeCounter;
//        #endif
        
        /** Menu bar for file save and load. */
        MenuBar menuBar;
        
        // Because we are not using modal dialogs....
        /** Audio Settings Component */
        SafePointer<DialogWindow> audioSettingsWindow;
        
        std::unique_ptr<FileChooser> fileChooser;
        
        //==============================================================================
        // Stuff for saving and loading and username
        String currentFilePath, username;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    };
} // namespace codetta
