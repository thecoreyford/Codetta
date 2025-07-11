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
        addAndMakeVisible (tutorialButton);
        addAndMakeVisible (browser);
        addAndMakeVisible (infoBar);
        addAndMakeVisible (bpmWidget);
        
        // setup audio playback
        audio.getMidiOutput().addListener (this);
        playbackControls.addListener (this);
        
        // setup tutorials
        tutIsShowing = false;
        tutorialButton.setButtonText ("?");
        tutorialButton.setColour (TextButton::ColourIds::buttonColourId,
                                 Colours::darkgrey);
        tutorialButton.setColour (TextButton::ColourIds::textColourOffId,
                                  Colours::white);
        tutorialButton.onClick = [this]
        {
            infoBar.updateContents ("Select a tutorial...");
            
            PopupMenu tutorials;
            tutorials.addItem (1, "Tutorial 1 - Basics");
            tutorials.addItem (2, "Tutorial 2 - Time");
            tutorials.addItem (3, "Tutorial 3 - Timbre");
            tutorials.addItem (4, "Tutorial 4 - Pitch");
            tutorials.addItem (5, "Hide Tutorials");

            const int result = tutorials.show();
            
            if (result != 0 && result != 5)
                tutIsShowing = true;
            
            if (result == 1){
                browser.goToURL ("https://thecoreyford.github.io/codetta/tutorials/tutorial1");
                infoBar.updateContents ("Tutorial 1 - Basics");
            }
            
            if (result == 2) {
                browser.goToURL ("https://thecoreyford.github.io/codetta/tutorials/tutorial2");
                infoBar.updateContents ("Tutorial 2 - Time");
            }
            
            if (result == 3) {
                browser.goToURL ("https://thecoreyford.github.io/codetta/tutorials/tutorial3");
                infoBar.updateContents ("Tutorial 3 - Timbre");
            }
            
            if (result == 4) {
                browser.goToURL ("https://thecoreyford.github.io/codetta/tutorials/tutorial4");
                infoBar.updateContents ("Tutorial 4 - Pitch");
            }
            
            if (result == 5 || result == 0) {
                infoBar.updateContents();
                tutIsShowing = false;
            }

            resized();
        };
        
        //======================================================================
        /////////////////////////////// Codetta ////////////////////////////////
        //======================================================================
        // Workspace setup
        auto blockFactory = std::make_shared<CodettaBlockFactory>();
        auto workspacePtr = viewport.getWorkspace();
        workspacePtr->injectBlockFactory (blockFactory);
        workspacePtr->setVisibleBounds (viewport.getViewArea());

        // Make block objects for toolbox
        FourFourBar barBlock;
        ThreeFourBar threeFour;
        TwoFourBar twoFour;
        FiveFourBar fiveFour;
        //--
        StartBlock startBlock;
        StartRepeat startRepeat;
        EndRepeat endRepeat;
        TempoSetter tempoSetter;
        TempoChanger tempoChanger;
        //--
        TrebleClef trebleClef;
        BassClef bassClef;
        //--
        Piano piano;
        Glockenspiel glock;
        AcousticGuitar acoustic;
        DistortionGuitar distortion;
        ElectricBass bass;
        Violin violin;
        Viola viola;
        Cello cello;
        Trumpet trumpet;
        AltoSax altoSax;
        TenorSax tenorSax;
        Bassoon bassoon;
        Flute flute;
        Pad pad;
        //--
        DynamicsBlock dynamics;
        DynamicsChanger dynChanger;

        // Toolbox setup
        toolbox.addCategory ("Basic", Colours::green);
        toolbox.addBlock ("Basic", &startBlock);
        toolbox.addBlock ("Basic", &barBlock);
        toolbox.addBlock ("Basic", &threeFour);
        toolbox.addBlock ("Basic", &twoFour);
        toolbox.addBlock ("Basic", &fiveFour);
        toolbox.addCategory("Instrument", Colours::rebeccapurple);
        toolbox.addBlock ("Instrument", &piano);
        toolbox.addBlock ("Instrument", &glock);
        toolbox.addBlock ("Instrument", &acoustic);
        toolbox.addBlock ("Instrument", &distortion);
        toolbox.addBlock ("Instrument", &bass);
        toolbox.addBlock ("Instrument", &violin);
        toolbox.addBlock ("Instrument", &viola);
        toolbox.addBlock ("Instrument", &cello);
        toolbox.addBlock ("Instrument", &trumpet);
        toolbox.addBlock ("Instrument", &altoSax);
        toolbox.addBlock ("Instrument", &tenorSax);
        toolbox.addBlock ("Instrument", &bassoon);
        toolbox.addBlock ("Instrument", &flute);
        toolbox.addBlock ("Instrument", &pad);
        toolbox.addCategory ("Time", Colours::yellow);
        toolbox.addBlock ("Time", &startRepeat);
        toolbox.addBlock ("Time", &endRepeat);
        toolbox.addBlock ("Time", &tempoSetter);
        toolbox.addBlock ("Time", &tempoChanger);
        toolbox.addCategory("Dynamics", Colours::hotpink);
        toolbox.addBlock ("Dynamics", &dynamics);
        toolbox.addBlock ("Dynamics", &dynChanger);
        toolbox.addCategory ("Pitch", Colours::blue);
        toolbox.addBlock ("Pitch", &trebleClef);
        toolbox.addBlock ("Pitch", &bassClef);

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
    }
    
    MainComponent::~MainComponent() {}
    
    //==============================================================================
    void MainComponent::paint (Graphics& g)
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::black);
    }
    
    void MainComponent::resized()
    {
        // This is called when the MainComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        
        //TODO: refactor "withTrimmed();
        
        auto main = getLocalBounds();
        
        auto temp = main;
        Rectangle<int> toolboxRect = temp.removeFromTop (getHeight() * 0.20);
        toolboxRect.removeFromLeft (getWidth() * 0.10);
        toolboxRect.removeFromRight (getWidth() * 0.05);
        
        // setup the bpm widget
        bpmWidget.setBounds (toolboxRect.withTrimmedBottom (toolboxRect.getHeight() * 0.82)
                             .withTrimmedLeft(toolboxRect.getWidth() * 0.85));
        
        // setup the browser
        if (tutIsShowing)
        {
            // setup the browser to show the right tutorial
            main.removeFromRight (getWidth() * 0.30);
            auto browserBounds = getLocalBounds();
            browserBounds.removeFromLeft (getWidth() * 0.70);
            browserBounds.removeFromTop (toolbox.getHeight());
            
            browser.setVisible (true);
            browser.setBounds (browserBounds);
        }
        else
        {
            browser.setVisible (false);
        }
        
        temp = getLocalBounds();
        Rectangle<int> playRect = temp.removeFromTop (getHeight() * 0.20);
        playRect.removeFromRight (getWidth() * 0.90);
        
        temp = getLocalBounds();
        Rectangle<int> tutRect = temp.removeFromTop (getHeight() * 0.20);
        tutRect.removeFromLeft (getWidth() * 0.95);

        temp = main;
        Rectangle<int> workspaceRect = temp.removeFromBottom (temp.getHeight() * 0.80);
        
        toolbox.setBounds (toolboxRect);
        tutorialButton.setBounds (tutRect);
        playbackControls.setBounds (playRect);
        viewport.setBounds (workspaceRect.withTrimmedBottom (workspaceRect.getHeight() * 0.05));
        infoBar.setBounds (workspaceRect.withTrimmedTop (workspaceRect.getHeight() * 0.95));
        viewport.getViewedComponent()->resized();
    }
    
    //==============================================================================
    
    void MainComponent::onStoppedPlaying()
    {
        playbackControls.onPlayingStopped();
    }
    
    //==========================================================================
    
    void MainComponent::onPlayClicked (TextButton& play, TextButton& stop)
    {
        // disable button until playback is finished
        play.setEnabled (false);
        stop.setEnabled (true);
        play.setButtonText ("Playing...");
        infoBar.updateContents ("Music playing!");
        MidiEventList::get().clearAllEvents();
        
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
        
//        MidiEventList::get().printEvents(); // Useful for debugging.
        audio.getMidiOutput().startPlaying();
    }
    
    void MainComponent::onStopClicked (TextButton& play, TextButton& stop)
    {
        infoBar.updateContents ("Music stopped!");
        stop.setEnabled (false);
        audio.getMidiOutput().setIsStopping (true);
    }

    //==========================================================================

} // namespace codetta


