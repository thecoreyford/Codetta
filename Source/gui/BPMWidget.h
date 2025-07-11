/*
  ==============================================================================

    BPMWidget.h
    Created: 20 Jan 2019 12:30:56pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../libs/MusicSync-Notation-Engine/Font.h"

//==============================================================================
/** Main namespace for codetta. */
namespace codetta
{
    /**
    *  Widget controlling a global tempo for the project.
    */
    class BPMWidget    : public Component
    {
    public:
        /** Constructor */
        BPMWidget();
        
        /** Destructor */
        ~BPMWidget();

        /**
         *  Paint method.
         *  @param the graphics context for this object.
         */
        void paint (Graphics&) override;
        
        /** Resized function. */
        void resized() override;

    private:
        /** Slider holding the tempo value for the object. */
        Slider tempoSlider {Slider::SliderStyle::IncDecButtons,
                            Slider::TextEntryBoxPosition::TextBoxLeft};
        
        /** Icon of globe */
        Image globe;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BPMWidget)
    };

} // namespace codetta
