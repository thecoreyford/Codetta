/*
  ==============================================================================

    MenuBar.cpp
    Created: 14 Oct 2019 12:52:53pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "MenuBar.h"

//==============================================================================

namespace codetta
{
    MenuBar::MenuBar()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
        listener = nullptr; 
        
        addAndMakeVisible (fileButton);
        Image normalButton = ImageCache::getFromMemory (BinaryData::fileButtonNormal_png, BinaryData::fileButtonNormal_pngSize);
        Image overButton = ImageCache::getFromMemory (BinaryData::fileButtonOver_png, BinaryData::fileButtonOver_pngSize);
        Image downButton = ImageCache::getFromMemory (BinaryData::fileButtonDown_png, BinaryData::fileButtonDown_pngSize);
        fileButton.setImages (false, true, true, normalButton, 1.0f, {}, overButton, 1.0f, {}, downButton, 1.0f, {});
        
        fileButton.onClick = [this]
        {
            PopupMenu options;
            options.addItem (1, "New");
            options.addItem (2, "Open");
            options.addItem (3, "Save");
            options.addItem (4, "Save As");
            options.addItem (5, "MIDI Export");
                       
            options.showMenuAsync(PopupMenu::Options(), [&](int result){
                if (result == 1) listener->menuBarPressed ("new");
                if (result == 2) listener->menuBarPressed ("load");
                if (result == 3) listener->menuBarPressed ("save");
                if (result == 4) listener->menuBarPressed ("saveAs");
                if (result == 5) listener->menuBarPressed ("midiExport");
            });
        };
    }

    MenuBar::~MenuBar()
    {
//        fileButton.setLookAndFeel (nullptr);
    }

    void MenuBar::paint (Graphics& g)
    {
//        g.fillAll (Colours::darkgrey);
    }

    void MenuBar::resized()
    {
//        fileButton.setBounds (getLocalBounds().withTrimmedRight (getWidth() * 0.93).reduced (1,1));
        fileButton.setBounds(getLocalBounds());
    }


    void MenuBar::MenuButtonLookAndFeel::drawButtonBackground (Graphics& g,
                                                               Button& button,
                                                               const Colour& bgColour,
                                                               bool isMouseOverButton,
                                                               bool isMouseDown)
    {
        auto buttonArea = button.getLocalBounds();
        
        g.setColour (Colours::darkgrey);
        if (isMouseOverButton) g.setColour(Colours::darkgrey.brighter());
        
        g.fillRect (buttonArea);
        
        g.setColour (Colours::darkgrey.darker());
        g.drawRect (buttonArea);
        
}
} // namespace codetta
