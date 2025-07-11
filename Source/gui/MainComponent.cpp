/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
/** Main namespace for codetta. */
namespace codetta
{
    MainComponent::MainComponent (Audio& audioRhs)    : audio (audioRhs),
                                                        toolbox (this),
                                                        infoBar (InfoBar::get())
    {
        // Component setup
        addAndMakeVisible (viewport);
        addAndMakeVisible (toolbox);
        addAndMakeVisible (playbackControls);
        addAndMakeVisible (browser);//<web?
        addAndMakeVisible (infoBar);
        addAndMakeVisible (bpmWidget);
        addAndMakeVisible (tutorialButton);
        addAndMakeVisible (menuBar);
        addAndMakeVisible (UndoWidget::get());
        addAndMakeVisible (browserExitButton);
        #ifdef PHD_STUDY_1
            addAndMakeVisible(DataLoggerViewer::get());
        #endif
        
        // Context + undo setup
        ContextTracker::get().setHelpTrigger (this);
        ContextTracker::get().setListener (this);
        UndoWidget::get().addListener (this);
        
        // setup audio playback
        audio.getMidiOutput().addListener (this);
        playbackControls.addListener (this);
        
        #if JUCE_IOS
            viewport.setScrollOnDragEnabled (false);
        #endif
        
        // add undo button
        // TODO: can this be done in the class
        UndoWidget::get().onClick = [this]{
            if (!audio.getMidiOutput().isTimerRunning())
                UndoWidget::get().rollback();
                LOG_STRING ("Undo button pressed");
                
                // Kill all sound!!!
                for (auto& i : {1,2,7,6,8,15,9})
                    audio.getMidiOutput().quickFireMidi(MidiMessage::allNotesOff(i));
        };
        
        // setup tutorials
        tutIsShowing = false;
        tutorialButton.setButtonText ("?");
        tutorialButton.setColour (TextButton::ColourIds::buttonColourId,
                                 Colours::darkgrey);
        tutorialButton.setColour (TextButton::ColourIds::textColourOffId,
                                  Colours::white);
        tutorialButton.addListener (this);
        
        
        browserExitButton.setColour(TextButton::ColourIds::buttonColourId,
                                    Colours::darkred);
        browserExitButton.setButtonText ("X");
        browserExitButton.onClick = [this] {
            tutIsShowing = false;
            resized();
        };
        
        
        // setup menubar
        menuBar.addListener (this);
        
        //setup keyboard
        setWantsKeyboardFocus(true);
        
        //======================================================================
        /////////////////////////////// Codetta ////////////////////////////////
        //======================================================================
        // Workspace setup
        auto blockFactory = std::make_shared<CodettaBlockFactory>();
        auto workspacePtr = viewport.getWorkspace();
        workspacePtr->injectBlockFactory (blockFactory);
        workspacePtr->setVisibleBounds (viewport.getViewArea());

        // Toolbox
        #ifdef AMY_VERSION_1
            toolbox.addCategory("Basic", Colours::green);
            toolbox.addBlock ("Basic", std::make_shared<Piano>());
//            toolbox.addBlock("Basic", std::make_shared<amy::PianoRollBar>());
            toolbox.addBlock("Basic", std::make_shared<amy::PainterlyBar>());
            toolbox.addBlock ("Basic", std::make_shared<StartRepeat>());
            toolbox.addBlock ("Basic", std::make_shared<EndRepeat>());
        #endif
        
        #if !defined AMY_VERSION_1
        toolbox.addCategory ("Basic", Colours::green);
        toolbox.addBlock ("Basic", std::make_shared<Piano>());
        toolbox.addBlock ("Basic", std::make_shared<TwoFourBar>());
        toolbox.addBlock ("Basic", std::make_shared<ThreeFourBar>());
        toolbox.addBlock ("Basic", std::make_shared<FourFourBar>());
        toolbox.addBlock ("Basic", std::make_shared<FiveFourBar>());
        #endif
       
        toolbox.addCategory("Instrument", Colours::rebeccapurple);
        toolbox.addBlock ("Instrument", std::make_shared<Piano>());
        toolbox.addBlock ("Instrument", std::make_shared<Glockenspiel>());
//        toolbox.addBlock ("Instrument", std::make_shared<AcousticGuitar>());
//        toolbox.addBlock ("Instrument", std::make_shared<DistortionGuitar>());
//        toolbox.addBlock ("Instrument", std::make_shared<ElectricBass>());
        toolbox.addBlock ("Instrument", std::make_shared<Violin>());
        toolbox.addBlock ("Instrument", std::make_shared<Viola>());
        toolbox.addBlock ("Instrument", std::make_shared<Cello>());
        toolbox.addBlock ("Instrument", std::make_shared<Trumpet>());
//        toolbox.addBlock ("Instrument", std::make_shared<AltoSax>());
//        toolbox.addBlock ("Instrument", std::make_shared<TenorSax>());
//        toolbox.addBlock ("Instrument", std::make_shared<Bassoon>());
////        toolbox.addBlock ("Instrument", std::make_shared<Flute>());
        toolbox.addBlock ("Instrument", std::make_shared<Pad>());
        
        #if !defined AMY_VERSION_1
        toolbox.addCategory ("Time", Colours::yellow);
        toolbox.addBlock ("Time", std::make_shared<StartRepeat>());
        toolbox.addBlock ("Time", std::make_shared<EndRepeat>());
        toolbox.addBlock ("Time", std::make_shared<TempoSetter>());
        toolbox.addBlock ("Time", std::make_shared<TempoChanger>());
        toolbox.addCategory("Dynamics", Colours::hotpink);
        toolbox.addBlock ("Dynamics", std::make_shared<DynamicsBlock>());
        toolbox.addBlock ("Dynamics", std::make_shared<DynamicsChanger>());
        toolbox.addCategory ("Pitch", Colours::blue);
        toolbox.addBlock ("Pitch", std::make_shared<TrebleClef>());
        toolbox.addBlock ("Pitch", std::make_shared<BassClef>());
        toolbox.addBlock ("Pitch", std::make_shared<PitchSetter>());
        #endif
        //======================================================================
        
        //======================================================================
        /////////////////////////////// juckly /////////////////////////////////
        //======================================================================
//        // Workspace setup
//        auto blockFactory = std::make_shared<juckly::DemoBlockFactory>();
//        workspace.injectBlockFactory (blockFactory);
//
//        // Make block objects for toolbox
//        juckly::DemoBlock block;
//        juckly::DemoStartBlock demoStartBlock;
//        juckly::DemoFunctionBlock functionBlock;
//        juckly::DemoFunctionBlockHolder functionBlockHolder;
//
//        // Toolbox setup
//        toolbox.addCategory ("Regular Blocks", Colours::green);
//        toolbox.addBlock ("Regular Blocks", &block);
//        toolbox.addBlock ("Regular Blocks", &functionBlockHolder);
//        toolbox.addCategory ("Start Blocks", Colours::red);
//        toolbox.addBlock ("Start Blocks", &demoStartBlock);
//        toolbox.addCategory ("Function Blocks", Colours::blue);
//        toolbox.addBlock ("Function Blocks", &functionBlock);
        //======================================================================
        
        // REF: (user33033, 2013)
        
        // Reset info bar on load.
        infoBar.updateContents();
        
        currentFilePath = "none";
        
        #ifdef PHD_STUDY_1
        DataLoggerViewer::get().update("0000");
        
        // setup file path
        currentFilePath = File::getSpecialLocation (File::SpecialLocationType::currentApplicationFile).getFullPathName().upToLastOccurrenceOf("/codetta", false, true)
                          + "/../Interaction-data";
        
//        DBG(currentFilePath);
        
//        jassert(false);
        
        // make the file
        File makeMe (currentFilePath);
        jassert(makeMe.createDirectory() == Result::ok());
        
        // add .xml to the path for saving the composition
        currentFilePath = currentFilePath + "/" + Time::getCurrentTime().toString (true, true).replaceCharacter(':', '-') + "-composition.xml";
        
		#if JUCE_WINDOWS
		currentFilePath = currentFilePath.replaceCharacter('/', '\\').toUTF8();
		#endif
		#endif
        
        #ifdef LOCK_LOGGING_TO_DOCUMENTS
        // Ask for a username, for the parent folder...
        EnterName usernameWindow;
        DialogWindow::showModalDialog ("Please enter your name:",
                                       &usernameWindow,
                                       this,
                                       Colours::lightgrey,
                                       true);
        
        username = usernameWindow.getText();
        
        // create the file path if not already made
        currentFilePath = File::getSpecialLocation (File::SpecialLocationType::userDocumentsDirectory)
                            .getFullPathName() + "/coreys-dataset/" + username;
        #ifdef WESSEX_LOG
		currentFilePath = currentFilePath.replace(File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory)
			.getFullPathName(), "S:/corey");
        #endif
        
        File makeMe (currentFilePath);
        
        jassert(makeMe.createDirectory() == Result::ok());
        
        // add .xml to the path for saving the composition
        currentFilePath = File::getSpecialLocation (File::SpecialLocationType::userDocumentsDirectory)
                                                    .getFullPathName() + "/coreys-dataset/" + username + "/" +
                                                    Time::getCurrentTime().toString (true, true).replaceCharacter(':', '-') 
													+ "-composition.xml";


        #ifdef WESSEX_LOG
		currentFilePath = currentFilePath.replace(File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory)
			.getFullPathName(), "S:/corey");
        #endif

		#if JUCE_WINDOWS
		currentFilePath = currentFilePath.replaceCharacter ('/', '\\');
		#endif

        #endif
        
        // Setup Trial Version Stuff
        #if defined CODETTA_TRIAL || defined USE_LOGGING
            timeCounter = 0;
        #endif
        startTimer (100); // for tutorials!
    }
    
    MainComponent::~MainComponent()
    {
        //Make sure audio settings window isn't still open
        if (audioSettingsWindow != nullptr){
            audioSettingsWindow->exitModalState (0);
            delete audioSettingsWindow;
        }
        
        #ifdef PHD_STUDY_1
            // add .xml to the path for saving the composition
        currentFilePath = File::getSpecialLocation (File::SpecialLocationType::currentApplicationFile).getFullPathName().upToLastOccurrenceOf("/codetta", false, true)
                          + "/../Interaction-data" + "/" + Time::getCurrentTime().toString (true, true).replaceCharacter(':', '-') + "final-composition.xml";
            
            #if JUCE_WINDOWS
            currentFilePath = currentFilePath.replaceCharacter('/', '\\').toUTF8();
            #endif

            save();
//			DBG(currentFilePath);
            WRITE_CSV(currentFilePath);
        #endif
        
        #ifdef LOCK_LOGGING_TO_DOCUMENTS
            // add .xml to the path for saving the composition
            currentFilePath = File::getSpecialLocation (File::SpecialLocationType::userDocumentsDirectory)
                                                        .getFullPathName() + "/coreys-dataset/" + username + "/" +
                                                        Time::getCurrentTime().toString (true, true).replaceCharacter(':', '-')
                                                        + "final-composition.xml";

            #ifdef WESSEX_LOG
            currentFilePath = currentFilePath.replace(File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory)
                .getFullPathName(), "S:/corey");
            #endif

            #if JUCE_WINDOWS
            currentFilePath = currentFilePath.replaceCharacter('/', '\\');
            #endif

            save();
            WRITE_CSV(currentFilePath);
        #endif
    }
    
    //==========================================================================
    void MainComponent::paint (Graphics& g)
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::black);
    }
    
    void MainComponent::resized()
    {
        auto helpString = ContextTracker::get().getRequestedHelp();
        if (helpString != "none"){
            tutIsShowing = true;
            //TODO: wrap this up into a factory, prehaps related to the Codetta one
            if (helpString == "Piano" ||
                helpString == "Glockenspiel" ||
                helpString == "Violin" ||
                helpString == "Viola" ||
                helpString == "Cello" ||
                helpString == "Trumpet" ||
                helpString == "Pad")
            {
                browser.goToURL ("https://codetta.codes/codetta/help/instrument");
            } else if (helpString.contains("Bar")){
                browser.goToURL ("https://codetta.codes/codetta/help/bars");
            } else if (helpString.contains("Repeat")){
                browser.goToURL ("https://codetta.codes/codetta/help/repeat");
            } else if (helpString.contains("Clef")){
                browser.goToURL ("https://codetta.codes/codetta/help/clefs");
            } else {
                browser.goToURL ("https://codetta.codes/codetta/help/" + helpString);
            }
            infoBar.updateContents ("Help opened for " + helpString);
            LOG_STRING ("Help opened for " + helpString);
        }
        
        //TODO: seriously, refactor this, it stinks!!!!!
        
        //======================================================================
        // variables
        const int menuBarHeight = 0;
        bool isIphone = false;
        auto main = getLocalBounds().withTrimmedTop(menuBarHeight);
        Rectangle<int> temp, tutRect;
        
        //======================================================================
        // lambdas
        auto checkIfIphone = [&]()
        {
            if (getWidth() <= 568/** based on iphone Xs Max*/)
                isIphone = true;
        };
        
        auto adjustCategoryNames = [&]()
        {
            if (isIphone)
            {
                #if JUCE_IOS
                    toolbox.changeCategoryName ("Instrument", "Inst");
                    toolbox.changeCategoryName ("Dynamics", "Dyn");
                #endif
            }
        };
    
        auto resizeTutorialButton = [&]()
        {
            // scale the tutorial button
            auto temp = getLocalBounds().withTrimmedTop(menuBarHeight); //no
            tutRect = temp.removeFromTop (getHeight() * 0.20);
            tutRect.removeFromLeft (getWidth() * 0.95);
        };
        
        auto resizeBrowser = [&]()
        {
            if (tutIsShowing)
            {
                // setup the browser to show the right tutorial
                main.removeFromRight (getWidth() * 0.30);
                auto browserBounds = getLocalBounds().withTrimmedTop(menuBarHeight); //no
                browserBounds.removeFromLeft (getWidth() * 0.70);
                browserBounds.removeFromTop (toolbox.getHeight());
                browserBounds.removeFromTop (28);
                
                browser.setVisible (true);//<web?
                browser.setBounds (browserBounds);//<web?
                
                // also setup the browser close button
                browserExitButton.setVisible (true);
                browserExitButton.setBounds(browserBounds.getX() + browserBounds.getWidth() - 30,
                                            browserBounds.getY() - 28, 30, 28);
            }
            else
            {
                browser.setVisible (false);//<web?
                browserExitButton.setVisible (false);
            }
            
        };
        
        auto resizeToolbox = [&]()
        {
            temp = main;
            Rectangle<int> toolboxRect = temp.removeFromTop (getHeight() * 0.20);
            toolboxRect.removeFromLeft (getWidth() * 0.10);
            if (!isIphone)
            {
                // give the toolbox room
                toolboxRect.removeFromRight (getWidth() * 0.05);
                
                resizeBrowser();
                resizeTutorialButton();
              
                // then set the bounds
                tutorialButton.setBounds (tutRect);
            }
            toolbox.setBounds (toolboxRect);
        };
        
        auto resizePlaybackControls = [&]()
        {
            // setup play button
            temp = getLocalBounds().withTrimmedTop(menuBarHeight);
            Rectangle<int> playRect = temp.removeFromTop (getHeight() * 0.20);
            playRect.removeFromRight (getWidth() * 0.90);
            playbackControls.setBounds (playRect); // moved
        };

        auto resizeWorkspace = [&]()
        {
            // setup the workspace
            temp = main;
            Rectangle<int> workspaceRect = temp.removeFromBottom (temp.getHeight() * 0.80);
            viewport.setBounds (workspaceRect.withTrimmedBottom (workspaceRect.getHeight() * 0.05));
            
            infoBar.setBounds (workspaceRect.withTrimmedTop (workspaceRect.getHeight() * 0.95));
            
            #ifdef PHD_STUDY_1
                DataLoggerViewer::get().setBounds (workspaceRect.withTrimmedTop (workspaceRect.getHeight() * 0.95).withTrimmedRight (workspaceRect.getWidth() * 0.85));
            #endif
            
            if (isIphone){
                auto workspace = dynamic_cast<juckly::Workspace*>(viewport.getViewedComponent());
                workspace->setBinIsShrunk (true);
            }
            viewport.getViewedComponent()->resized();
        };
        
        auto resizeBPMWidget = [&]()
        {
            Rectangle<int> widgetBounds;
            widgetBounds.setSize (175, 28);
            widgetBounds.setY (menuBarHeight);
            if (!isIphone)
                widgetBounds.setX (tutorialButton.getX() - widgetBounds.getWidth());
            else
                widgetBounds.setX (getWidth() - widgetBounds.getWidth());
            
            bpmWidget.setBounds (widgetBounds);
        };
        
        auto resizeUndoButton = [&]()
        {
            Rectangle<int> undoBounds;
            undoBounds.setSize (30, 28); //TODO: don't hardcode
            undoBounds.setY (menuBarHeight);
            if (!isIphone)
                undoBounds.setX (bpmWidget.getX() - undoBounds.getWidth() - 1);
            else
                undoBounds.setX (getWidth() - undoBounds.getWidth() - 1);
            UndoWidget::get().setBounds (undoBounds);
        };
        
        auto resizeMenuBar = [&]()
        {
            Rectangle<int> menuBarSize;
            menuBarSize.setSize (30, 28); //TODO: don't hardcode
            menuBarSize.setY (menuBarHeight);
            if (!isIphone)
                menuBarSize.setX (bpmWidget.getX() - (menuBarSize.getWidth() * 2) - 2);
            else
                menuBarSize.setX (getWidth() - (menuBarSize.getWidth() * 2) - 2);
            menuBar.setBounds (menuBarSize);
        };
        
        //======================================================================
        // Call lambdas
        checkIfIphone();
        adjustCategoryNames();
        resizeToolbox(); // also sets browser & tut buttons
        resizePlaybackControls();
        resizeWorkspace();
        resizeBPMWidget();
        resizeUndoButton();
        resizeMenuBar();
    }

    //==========================================================================
    
    void MainComponent::loopOverBlocks()
    {
        auto workspace = dynamic_cast<juckly::Workspace*>(viewport.getViewedComponent());
        jassert (workspace != nullptr); //<The viewport didn't cast to a Workspace, check ptrs!!!
        
        // Get all start blocks from the workspace
        auto& heads = workspace->getStartBlocks();
        
        // for each start block
        for (int i = 0 ; i < heads.size(); ++i)
        {
            auto current = heads[i];
            bool isNull = true;
            
            do
            {
                // presume we are pointing to null
                isNull = true;
                
                if (current != nullptr)
                {
                    isNull = false;
                    current->doAction();
                    current = current->getConnection().getNextNode();
                }
            } while (!isNull);
        }
    }
    
    //==========================================================================
    
    void MainComponent::onStoppedPlaying()
    {
        LOG_STRING("Playing Stoppped");
        playbackControls.onPlayingStopped();
//        Arcs::get().stopDrawing();
        LiveGuiPlayback::get().setAllNotesBlack(); //< moved to here to fix bug
                                                   //TODO: difficult to test!!!
    }
    
    //==========================================================================
    
    void MainComponent::onPlayClicked (TextButton& play, TextButton& stop)
    {
        // clear arcs
        LiveGuiPlayback::get().clearTimestampedArcList();
        
        LOG_STRING("Play Clicked");
        
        // disable button until playback is finished
        play.setEnabled (false);
        stop.setEnabled (true);
        play.setButtonText ("Playing...");
        infoBar.updateContents ("Music playing!");
        MidiEventList::get().clearAllEvents();
        LiveGuiPlayback::get().clearAllEvents();
        
        // Loop over and run all of the blocks
        loopOverBlocks();
        
//        MidiEventList::get().printEvents(); // Useful for debugging.
        audio.getMidiOutput().startPlaying();
    }
    
    void MainComponent::onStopClicked (TextButton& play, TextButton& stop)
    {
        LOG_STRING("Stop Clicked");
        infoBar.updateContents ("Music stopped!");
        audio.getMidiOutput().setIsStopping (true);
        stop.setEnabled (false);
//        Arcs::get().stopDrawing();
        
    }

    //==========================================================================
    
    void MainComponent::buttonClicked (Button* button)
    {
        if (&tutorialButton == button)
        {
            LOG_STRING("Tutorial Button Clicked");
            infoBar.updateContents ("Select a tutorial...");
            
            PopupMenu tutorials;
            #if !defined AMY_VERSION_1
            tutorials.addItem (1, "Tutorial 1 - Basics");
            tutorials.addItem (2, "Tutorial 2 - Time");
            tutorials.addItem (3, "Tutorial 3 - Timbre");
            tutorials.addItem (4, "Tutorial 4 - Pitch");
            tutorials.addItem (5, "Phd Study 1 Tutorial");
            #endif
            
            #ifdef AMY_VERSION_1
            tutorials.addItem (6, "Questionaire");
            #endif
            tutorials.addItem (7, "Audio Settings");
            #if !defined AMY_VERSION_1
            tutorials.addItem (8, "Hide Tutorials");
            #endif
            
            tutorials.showMenuAsync(PopupMenu::Options(), [&](int result){
                if (result != 0 && result != 7 && result != 6)
                    tutIsShowing = true;
                    
                if (result == 1){
                    browser.goToURL ("https://thecoreyford.github.io/codetta-tutorials/tutorial1");//<web?
                    infoBar.updateContents ("Tutorial 1 - Basics");
                    LOG_STRING("Tutorial 1 Opened");
                }
                
                if (result == 2) {
                    browser.goToURL ("https://thecoreyford.github.io/codetta-tutorials/tutorial2");//<web?
                    infoBar.updateContents ("Tutorial 2 - Time");
                    LOG_STRING("Tutorial 2 Opened");
                }
                
                if (result == 3) {
                    browser.goToURL ("https://thecoreyford.github.io/codetta-tutorials/tutorial3");//<web?
                    infoBar.updateContents ("Tutorial 3 - Timbre");
                    LOG_STRING("Tutorial 3 Opened");
                }
                
                if (result == 4) {
                    browser.goToURL ("https://thecoreyford.github.io/codetta-tutorials/tutorial4");//<web?
                    infoBar.updateContents ("Tutorial 4 - Pitch");
                    LOG_STRING("Tutorial 4 Opened");
                }
                
                if (result == 6) {
                    LicenseInfo licenseInfo("https://thecoreyford.github.io/codetta-tutorials/questionaire_amy");
                    licenseInfo.setWantsKeyboardFocus(true);
                    DialogWindow::showDialog ("Questionaire",
                                                   &licenseInfo,
                                                   this,
                                                   Colours::lightgrey,
                                                   true);
                    LOG_STRING("Questionaire opened");
                }
                
                if (result == 5) {
                    browser.goToURL ("https://thecoreyford.github.io/codetta-tutorials/tutorial_phd1b");//<web?
                    infoBar.updateContents ("Phd Study 1 Tutorial");
                    LOG_STRING("Phd Study 1 Tutorial Opened");
                }
                
                if (result == 7) {
                    showAudioSettings();
                    infoBar.updateContents ("Audio Settings Open...");
                    LOG_STRING("Audio Settings Opened");
                }
                
                if (result == 8 /*|| result == 5 */|| result == 0) {
                    infoBar.updateContents();
                    tutIsShowing = false;
                    LOG_STRING("Tutorial Pane Closed");
                }
                
                resized(); //make browser visible!
            });
        }
    }
    
    //==========================================================================
    
    void MainComponent::timerCallback()
    {
#ifdef CODETTA_SHOW_TUTORIALS_FIRST
        // Start tutorial 1 by default
        
        if (timeCounter < 1)
        {
            timeCounter++; // to stop duplicates
            [&](){
                // Startup Tutorial 1 by default!
                tutIsShowing = true;
                #if defined PHD_STUDY_1
                    browser.goToURL ("https://thecoreyford.github.io/codetta-tutorials/tutorial_phd1b");
                    infoBar.updateContents ("Phd Study 1 Tutorial");
                #else
                    browser.goToURL ("https://thecoreyford.github.io/codetta-tutorials/tutorial1");
                    infoBar.updateContents ("Tutorial 1 - Basics");
                #endif
                
                resized();
            }();
            
            #ifdef CODETTA_TRIAL
                // Force users into choosing the correct sound setting!
                showAudioSettings();
            #endif
            
            RESET_LOG(); // ensure data logging dosen't contain preamble material
        }
#endif
                
#if !defined CODETTA_TRIAL && !defined USE_LOGGING
        stopTimer();
#endif
        
#ifdef CODETTA_TRIAL
        if (timeCounter == ( (10/*mins*/
                              * 60
                              * 1000)
                            / 100) )
        {
            timeCounter++; // to stop duplicates!
            AlertWindow::showMessageBox (AlertWindow::InfoIcon,
                                         "Time Remaning:",
                                         "You are half way through with 10 mins left to explore! :)");
        }
        
        if (timeCounter == ( (15/*mins*/
                              * 60
                              * 1000)
                            / 100) )
        {
            timeCounter++; //to stop duplicates
            // shutdown the app
            AlertWindow::showMessageBox (AlertWindow::InfoIcon,
                                         "Time Remaning:",
                                         "You have 5 mins left to explore! :)");
        }
        
        if (timeCounter == ( (20/*mins*/
                              * 60
                              * 1000)
                              / 100) )
        {
            timeCounter++; //to stop duplicates
            AlertWindow::showMessageBox (AlertWindow::InfoIcon,
                                              "Congratulations!",
                                              "Your time with codetta has finished!" /*" \n Please return to the questionaire in your browser!"*/);
            // shutdown the app
            JUCEApplication::getInstance()->systemRequestedQuit();
        }
#endif
        
#ifdef USE_LOGGING
        // save every 3 mins
        if (timeCounter == ( (3/*mins*/
                                     * 60
                                     * 1000)
                                   / 100) )
               {
                  timeCounter = 0; // to stop duplicates and reset for every 3 mins
                  
                   // add .xml to the path for saving the composition
                   #if defined LOCK_LOGGING_TO_DOCUMENTS
                   currentFilePath = File::getSpecialLocation (File::SpecialLocationType::userDocumentsDirectory)
                                      .getFullPathName() + "/coreys-dataset/" + username + "/" +
                                      Time::getCurrentTime().toString (true, true).replaceCharacter(':', '-') 
									  + "-composition.xml";
                    #endif
                   
                   
                 #if defined PHD_STUDY_1
                   currentFilePath = File::getSpecialLocation (File::SpecialLocationType::currentApplicationFile).getFullPathName().upToLastOccurrenceOf("/codetta", false, true) + "/../Interaction-data/" + Time::getCurrentTime().toString (true, true).replaceCharacter(':', '-') + "-composition.xml";
                #endif
				   


				#ifdef WESSEX_LOG
				  currentFilePath = currentFilePath.replace(File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory)
					  .getFullPathName(), "S:/corey");
				#endif


				#if JUCE_WINDOWS
				  currentFilePath = currentFilePath.replaceCharacter('/', '\\').toUTF8();
				#endif
                   
                   save();
				   WRITE_CSV(currentFilePath);
               }
#endif
        
        if (timeCounter == 0)
        {
            UndoWidget::get().clear(); // Ensure that the stack is empty...
            UndoWidget::get().updateUndoStack(); //... and then give it an initial value.
        }
        #if !defined USE_LOGGING
        else
        {
            stopTimer();
        } //TODO: remove for logging
        #endif
        
        timeCounter++;
//        DBG(timeCounter);
    }
    
    void MainComponent::showAudioSettings()
    {
        AudioDeviceSelectorComponent* audioSettingsComp = new AudioDeviceSelectorComponent
                                                              (audio.getAudioDeviceManager(),
                                                              0, 2, 2, 2, true, true, true, false);
        audioSettingsComp->setSize (450, 350); //TODO: make this work...
        
        
        DialogWindow::LaunchOptions options;
        options.content.setOwned (audioSettingsComp);
        
        options.dialogTitle                   = "Audio Settings";
        options.dialogBackgroundColour        = Colours::grey;
        options.escapeKeyTriggersCloseButton  = true;
        options.useNativeTitleBar             = false;
        options.resizable                     = true;
        
        audioSettingsWindow = options.launchAsync();
         if (audioSettingsWindow != nullptr)
            audioSettingsWindow->centreWithSize (450, 350);
    }

    //==========================================================================
    
    void MainComponent::menuBarPressed (const String& item)
    {
        LOG_STRING("Menu Bar Pressed");
        onStopClicked (playbackControls.getPlayButton(),
                       playbackControls.getStopButton());
        
        if (item == "new"){
            #if !defined LOCK_LOGGING_TO_DOCUMENTS || !defined PHD_STUDY_1
                currentFilePath = "none";
            #endif
            viewport.getWorkspace()->clearWorkstation();
            LOG_STRING("New Clicked");
        } //TODO: are you sure?
        if (item == "load") load(); LOG_STRING("Load Clicked");
        if (item == "save") save(); LOG_STRING("Save Clicked");
        if (item == "saveAs"){
            #if !defined LOCK_LOGGING_TO_DOCUMENTS || !defined PHD_STUDY_1
                currentFilePath = "none";
            #endif
            save();
            LOG_STRING("Save as clicked");
        }
        if (item == "midiExport") midiExport(); LOG_STRING("MIDI Exported");
    }

    void MainComponent::save()
    {
        if (currentFilePath == "none") // this is set on save as!
        {
            fileChooser.reset (new FileChooser ("Please select the location you want to save your file...",
                                                File::getSpecialLocation (File::userHomeDirectory),
                                                "*.xml"));
        
            fileChooser->launchAsync(FileBrowserComponent::saveMode |
                                     FileBrowserComponent::canSelectFiles,
                                     [&] (const FileChooser& chooser)
                                     {
                                         auto result = chooser.getURLResult();
                                         auto name = result.isEmpty() ? "oh no!"
                                                                      : (result.isLocalFile() ? result.getLocalFile().getFullPathName()
                                                                                              : result.toString (true));
                                         if (name == "oh no!"){
                                             infoBar.updateContents ("Error saving file, please try again!!!");
                                         }else{
                                            writeSave(new File(name));
                                         }
                                     });
        }
        else
        {
            writeSave(new File(currentFilePath));
        }
    }

    void MainComponent::writeSave (File* savedFile)
    {
//        DBG(savedFile->getFullPathName());
        XmlElement savedXML = storeXmlElement();
        
        
        #if defined JUCE_MAC && defined USE_LOGGING
            String myString = (String)"curl -X POST https://content.dropboxapi.com/2/files/upload \\\n"
                                + (String)"    --header \"Authorization: Bearer q4Cy6jD0XWgAAAAAAAAAAexsOAtOD50euF0z6ca7sg4EuQLJ8BA31By2i_hxdZCG"
            + (String)"\" \\\n" + "    --header \"Dropbox-API-Arg: {\\\"path\\\": \\\"/Phd-3/"
            + savedFile->getFileName()
            + "\\\"}\" \\\n"
            + (String)"    --header \"Content-Type: application/octet-stream\" \\\n"
            + (String)"    -d \""
            + savedXML.toString()
            + (String)"\"";

            system (myString.toRawUTF8());
        #endif
        
        //--saved?----------------------------------------------------------
        if (!savedXML.writeToFile (*savedFile, String()))
        {
            jassert(false); //HECK!
        }
        else
        {
            // update info bar
            infoBar.updateContents ("File " + savedFile->getFileName() + " saved!");
            // store the sucessfull saved path
            currentFilePath = savedFile->getFullPathName();
        }
    }

    XmlElement MainComponent::storeXmlElement()
    {
        // Create and store a dummy file
        XmlElement savedXML ("codetta");
         
        //--bpm widget------------------------------------------------------
        XmlElement* tempoElm = new XmlElement ("GlobalTempo");
        tempoElm->setAttribute ("bpm", bpmWidget.getTempo());
        savedXML.addChildElement (tempoElm);
         
        //--blocks----------------------------------------------------------
        auto workspacePtr = viewport.getWorkspace();
        auto& heads = workspacePtr->getStartBlocks();
        auto& other = workspacePtr->getOtherBlocks();
         
        // for each start block...
        for (auto current : heads)
        {
            XmlElement* blockElm = new XmlElement ("Block");
            blockElm->setAttribute ("id", current->getID());
            blockElm->setAttribute ("x",  current->getX()
                                     + (current->getWidth() * 0.5));
            blockElm->setAttribute ("y",  current->getY()
                                     + (current->getHeight() * 0.5));
             
            // do block specific actions
            current->doSaveOrLoad (blockElm,
                                   juckly::Block::FileManipulator::save);
             
            savedXML.addChildElement (blockElm);
         }
         
         // for each other block...
         for (auto current : other)
         {
             XmlElement* blockElm = new XmlElement ("Block");
             blockElm->setAttribute ("id", current->getID());
             blockElm->setAttribute ("x",  current->getX()
                                      + (current->getWidth() * 0.5));
             blockElm->setAttribute ("y",  current->getY()
                                      + (current->getHeight() * 0.5));
             
             // do block specific actions
             current->doSaveOrLoad (blockElm, juckly::Block::FileManipulator::save);
                 
             savedXML.addChildElement (blockElm);
         }
        
        return savedXML;
    }

    void MainComponent::load()
    {
        fileChooser.reset (new FileChooser ("Please select the location you want to save your file...",
                                                File::getSpecialLocation (File::userHomeDirectory),
                                                "*.xml"));
        
        fileChooser->launchAsync (FileBrowserComponent::openMode |
                                  FileBrowserComponent::canSelectFiles,
                                  [&](const FileChooser& chooser)
                                  {
                                        auto result = chooser.getURLResult();
                                        auto name = result.isEmpty() ? "oh no!"
                                                                     : (result.isLocalFile() ? result.getLocalFile().getFullPathName()
                                                                                             : result.toString (true));
                                        if (name != "oh no!")
                                        {
                                            // clear workspace
                                            viewport.getWorkspace()->clearWorkstation();

                                            // get file
                                            File loadedFile (name);

                                            XmlDocument loadedXML (loadedFile);
                                            std::unique_ptr<XmlElement> loadedElm = loadedXML.getDocumentElement();

                                            loadXmlElement(*loadedElm.get());

                                            // update info bar
                                            infoBar.updateContents ("File " + loadedFile.getFileName() + " loaded!");
                                            currentFilePath = loadedFile.getFullPathName();
                                        }
                                        else
                                        {
                                            jassert("not a codetta file bro!");//TODO: message box
                                        }
                                  });
    }

    void MainComponent::loadXmlElement(XmlElement loadedElm)
    {
        if (loadedElm.hasTagName ("codetta")){
            // Great! This is a codetta document - wahoo!
             forEachXmlChildElement (loadedElm, e) {
                 if (e->hasTagName ("GlobalTempo"))
                     bpmWidget.setTempo (e->getIntAttribute("bpm"));

                 if (e->hasTagName ("Block")) {
                     auto workspace = viewport.getWorkspace();
                     Point<int> dragPoint (e->getIntAttribute("x"),
                                           e->getIntAttribute("y"));
                    DragAndDropTarget::SourceDetails blockSource
                                                         (e->getStringAttribute("id"),
                                                          this, dragPoint);
                     workspace->itemDropped (blockSource);

                     // load block specific stuff
                     workspace->getLastAddedBlock()->doSaveOrLoad (e, juckly::Block::FileManipulator::load);
                 }
             }

            //--do connections also------------------------------------------

            auto workspace = viewport.getWorkspace();
            auto& heads = workspace->getStartBlocks();
            // for each start block
            for (int i = 0 ; i < heads.size(); ++i) {
                auto current = heads[i];
                bool isNull = true;
                do {
                    isNull = true; // presume we are pointing to null
                    if (current != nullptr)
                    {
                        isNull = false;
                        current->setLoading(true);
                        workspace->checkCollisions (current, false);
                        current->setLoading(false);
                        current = current->getConnection().getNextNode();
                    }
                } while (!isNull);
            }
        }
    }



    void MainComponent::midiExport()
    {
        fileChooser.reset (new FileChooser ("Please select the location you want to save your file...",
                                            File::getSpecialLocation (File::userHomeDirectory),
                                            "*.midi"));
    
        fileChooser->launchAsync(FileBrowserComponent::saveMode
                                 | FileBrowserComponent::canSelectFiles,
                                 [&] (const FileChooser& chooser)
                                 {
                                     auto result = chooser.getURLResult();
                                     auto name = result.isEmpty() ? "oh no!"
                                                                  : (result.isLocalFile() ? result.getLocalFile().getFullPathName()
                                                                                          : result.toString (true));
                                     if (name == "oh no!")
                                     {
                                         infoBar.updateContents ("Error saving file, please try again!!!");
                                     }
                                     else
                                     {
                                         //Clean the MidiEvent List
                                         MidiEventList::get().clearAllEvents();
                                         
                                         // Update the MIDI event list
                                         loopOverBlocks();
                                         
                                         // Write the MIDI file
                                         MidiEventList::get().exportToFile(File(name));
                                     }
                                 });
    }

    XmlElement MainComponent::onUndoCommand(bool update, XmlElement elm)
    {
        if (update == true)
        {
            XmlElement element = storeXmlElement();
            return element;
        }
        else
        {
            UndoWidget::get().setIsUndoing(true);
            viewport.getWorkspace()->clearWorkstation();
            loadXmlElement(elm);
            UndoWidget::get().setIsUndoing(false);
            return elm; //< just to silence the warning
        }
    }

    void MainComponent::areYouSureYouWantToQuitWithoutSaving()
    {
        #ifdef USE_LOGGING
        while (currentFilePath == "none"){
        #endif
        //----------------------------------------------------------------------
        AlertWindow::showOkCancelBox (AlertWindow::WarningIcon,
                                      "Have you saved your work?",
                                      "You could loose your work if you haven't saved it.",
                                      "Save my work!",
                                      "Exit anyway!",
                                      {},
                                      ModalCallbackFunction::forComponent (alertBoxResultChosen, this));
        //----------------------------------------------------------------------
        #ifdef USE_LOGGING
        }
        #endif
    }

    //============
    #ifdef CODETTA_KEYBOARD_CONTROLS
    bool MainComponent::keyPressed (const KeyPress &k)
    {
        char key = k.getTextCharacter();
        switch (key){
            case 'r':{
                auto bar = dynamic_cast<MusiSyncEng::Bar*>(ContextTracker::get().getLastUsed("Bar"));
                if (bar != nullptr)
                {
                    // try and just duplicate the last note
                    auto success = bar->duplicateLastNote();
                    if(!success) //if we can't do this as the bar is full
                    {
                        auto thisBlock = dynamic_cast<juckly::Block*>(bar->getParentComponent());
                        auto nextBlock = thisBlock->getConnection().getNextNode();
                        
                        // we will either move onto the next bar...
                        if (nextBlock != nullptr)
                        {
                            if (nextBlock->getID().contains("Bar"))
                            {
                                ContextTracker::get().updateLastUsed ("Bar", nextBlock->getInternalUI());
                            }
                        }
                        else //... or we will create a new identical one
                        {
                            auto workspacePtr = viewport.getWorkspace();
                            
                            std::unique_ptr<XmlElement> blockElm = std::make_unique<XmlElement>("Block");
                            blockElm->setAttribute ("id", thisBlock->getID());
                            blockElm->setAttribute ("width", thisBlock->getWidth());
                            blockElm->setAttribute ("height", thisBlock->getHeight());
                            int x = thisBlock->getX() + (thisBlock->getWidth() * 1.45);
                            int y = (thisBlock->getY()) + (thisBlock->getHeight() * 0.5);
                            Point<int> coords {x, y};
                            workspacePtr->injectBlockFromDescription (blockElm, coords);
                            workspacePtr->checkCollisions (thisBlock, false);
                        }
                    }
                }
                break;
            }
            case '\'':{
                auto note = dynamic_cast<MusiSyncEng::Note*>(ContextTracker::get().getLastUsed("Note"));
                if (note != nullptr){
                    int pitch = note->getPitchOffset();
                    dynamic_cast<MusiSyncEng::Note*>(note)->setPitchOffset (pitch+1);
                }
                break;
            }
            case '/':{
                auto note = ContextTracker::get().getLastUsed("Note");
                if (note != nullptr){
                    int pitch = dynamic_cast<MusiSyncEng::Note*>(note)->getPitchOffset();
                    dynamic_cast<MusiSyncEng::Note*>(note)->setPitchOffset (pitch-1);
                }
                break;
            }
            case '[':{
                auto bar = ContextTracker::get().getLastUsed("Bar");
                if (bar != nullptr)
                    dynamic_cast<MusiSyncEng::Bar*>(bar)->onMinusClicked();
                break;
            }
            case ']':{
                auto bar = ContextTracker::get().getLastUsed("Bar");
                if (bar != nullptr)
                    dynamic_cast<MusiSyncEng::Bar*>(bar)->triggerPlus();
                break;
            }
            case ' ':{
                if (playbackControls.getPlayButton().isEnabled()) {
                    onPlayClicked (playbackControls.getPlayButton(),
                                   playbackControls.getStopButton());
                }
                else {
                    onStopClicked (playbackControls.getPlayButton(),
                                   playbackControls.getStopButton());
                }
                break;
            }
        }
        
        return true;
    }
    #endif

    void MainComponent::onContext (Component* comp)
    {
        // Figure out the insturment
        auto note = dynamic_cast<MusiSyncEng::Note*>(comp);
        auto bar = dynamic_cast<MusiSyncEng::Bar*>(note->getParentBar());
        auto block = dynamic_cast<juckly::Block*>(bar->getParent());
        String instrumentID;
        do
        {
            instrumentID = block->getID();
            block = block->getConnection().getPreviousNode();
        }
        while(block != nullptr);
        
        // Figure out what channel this is
        int channel = 1;
        if (instrumentID.contains("Piano")) channel = 1;
        if (instrumentID.contains("Glock")) channel = 2;
        if (instrumentID.contains("Viola")) channel = 7;
        if (instrumentID.contains("Violin")) channel = 6;
        if (instrumentID.contains("Cello")) channel = 8;
        if (instrumentID.contains("Pad")) channel = 15;
        if (instrumentID.contains("Trumpet")) channel = 9;
        
        // Fire out notes
        int value = note->getMusicalValue();
        audio.getMidiOutput().quickFireMidi(MidiMessage::noteOn(channel, value, (uint8)35));
        Timer::callAfterDelay(300, [&]() // Silence after loop
        {
            for (auto& i : {1,2,7,6,8,15,9})
            {
                audio.getMidiOutput().quickFireMidi(MidiMessage::allNotesOff(i));
            }
        });
    }
    
} // namespace codetta
