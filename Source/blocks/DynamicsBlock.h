/*
  ==============================================================================

    DynamicsBlock.h
    Created: 12 Jan 2019 3:01:53pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "../audio/Audio.h"
#include "../libs/juckly/client/Block.h"
#include "../gui/InfoBar.h"

//==============================================================================

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  A block that sets the dynamics (velocity).
     */
    class DynamicsBlock    : public juckly::Block
    {
    public:
        /** Default constructor. */
        DynamicsBlock () : juckly::Block ( std::make_unique<juckly::CommandBlockSettings>
                                        ("DynamicsBlock",
                                         ImageCache::getFromMemory (BinaryData::dynamicsBlockIcon_png, BinaryData::dynamicsBlockIcon_pngSize),
                                         ImageCache::getFromMemory (BinaryData::pinkBlock_png, BinaryData::pinkBlock_pngSize),
                                         false /* not a start node */,
                                         new ComboBox(),
                                         false /* dosen't take a parameter */
                                         )
                                        )
        {
            auto comboBox = dynamic_cast<ComboBox*>(getInternalUI());
            jassert (comboBox != nullptr); // Cast didn't work!
            comboBox->addItem ("pp", 1);
            comboBox->addItem ("p", 2);
            comboBox->addItem ("mp", 3);
            comboBox->addItem ("mf", 4);
            comboBox->addItem ("f", 5);
            comboBox->addItem ("ff", 6);
            comboBox->setSelectedId (3);

            // update logger with lambda
            comboBox->onChange = [this]
            {
                auto comboBoxL = dynamic_cast<ComboBox*>(getInternalUI());
                jassert (comboBoxL != nullptr); // Cast didn't work!
                switch (comboBoxL->getSelectedItemIndex())
                {
                    case 0: InfoBar::get().updateContents ("Dynamics set pianissimo (very quiet)!"); break;
                    case 1: InfoBar::get().updateContents ("Dynamics set piano (quiet)!"); break;
                    case 2: InfoBar::get().updateContents ("Dynamics set mezzo piano (mildy quiet)!"); break;
                    case 3: InfoBar::get().updateContents ("Dynamics set mezzo forte (mildy loud)!"); break;
                    case 4: InfoBar::get().updateContents ("Dynamics set forte (loud)!"); break;
                    case 5: InfoBar::get().updateContents ("Dynamics set fortissimo (very loud)!"); break;
                }
            };
        }
        
        /** Changes the velocity to the correct value based on the combo box index. */
        void doAction() override
        {
            auto comboBox = dynamic_cast<ComboBox*>(getInternalUI());
            jassert (comboBox != nullptr); // Cast didn't work!
            PlaybackSettings::get().setVelocity ((comboBox->getSelectedItemIndex()+1) * 21);
        }
        
    private:
    };
} // namespace
