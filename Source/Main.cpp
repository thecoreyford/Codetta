/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "gui/MainComponent.h"
#include "audio/Audio.h"

//==============================================================================
// Discussion on icon implementation is found here...
// REF: (fordc004, 2019)

/** Main namespace for codetta. */
namespace codetta
{
    class CodettaApplication  : public JUCEApplication
    {
    public:
        //==============================================================================
        CodettaApplication() {}
        
        const String getApplicationName() override       { return ProjectInfo::projectName; }
        const String getApplicationVersion() override    { return ProjectInfo::versionString; }
        bool moreThanOneInstanceAllowed() override       { return true; }
        
        //==============================================================================
        void initialise (const String& commandLine) override
        {
            // This method is where you should put your application's initialisation code..
            
            mainWindow.reset (new MainWindow (getApplicationName(), audio));
        }
        
        void shutdown() override
        {
            // Add your application's shutdown code here..
            
            mainWindow = nullptr; // (deletes our window)
        }
        
        //==============================================================================
        void systemRequestedQuit() override
        {
            // This is called when the app is being asked to quit: you can ignore this
            // request and let the app carry on running, or call quit() to allow the app to close.
            quit();
        }
        
        void anotherInstanceStarted (const String& commandLine) override
        {
            // When another instance of the app is launched while this one is running,
            // this method is invoked, and the commandLine parameter tells you what
            // the other instance's command-line arguments were.
        }
        
        //==============================================================================
        /*
         This class implements the desktop window that contains an instance of
         our MainComponent class.
         */
        class MainWindow    : public DocumentWindow
        {
        public:
            MainWindow (String name, Audio& audio)  :    DocumentWindow (name,
                                                         Desktop::getInstance().getDefaultLookAndFeel()
                                                        .findColour (ResizableWindow::backgroundColourId),
                                                         DocumentWindow::allButtons)
            {
                setUsingNativeTitleBar (true);
                mainComponent.reset (new MainComponent(audio));
                
                #if JUCE_IOS
                    mainComponent->setSize (568, 320);
                    setFullScreen (true); //320x568
                #else
                    mainComponent->setSize (1366, 768);
                #endif
                
                setResizable (true, false);
                setContentNonOwned (mainComponent.get(), true);

                centreWithSize (getWidth(), getHeight());
                setVisible (true);
            }
            
            void closeButtonPressed() override
            {
                // This is called when the user tries to close this window. Here, we'll just
                // ask the app to quit when this happens, but you can change this to do
                // whatever you need.
                JUCEApplication::getInstance()->systemRequestedQuit();
            }
            
            /* Note: Be careful if you override any DocumentWindow methods - the base
             class uses a lot of them, so by overriding you might break its functionality.
             It's best to do all your work in your content component instead, but if
             you really have to override any DocumentWindow methods, make sure your
             subclass also calls the superclass's method.
             */
            
        private:
            std::unique_ptr<MainComponent> mainComponent;
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
        };
        
    private:
        /** Single GUI instance. */
        std::unique_ptr<MainWindow> mainWindow;
        
        /** Single audio instance. */
        Audio audio;
    };
    
    //==============================================================================
    // This macro generates the main() routine that launches the app.
    START_JUCE_APPLICATION (CodettaApplication)
}
