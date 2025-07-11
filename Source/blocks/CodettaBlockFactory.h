/*
  ==============================================================================

    CodettaBlockFactory.h
    Created: 17 Sep 2018 11:46:42am
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../libs/juckly/client/BlockFactory.h"
#include "../gui/InfoBar.h"
#include "Blocks.h"

//==============================================================================

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  The block factory for creating adding blocks to Codettas workspace.
     */
    class CodettaBlockFactory    : public juckly::BlockFactory
    {
    public:
        /**
         *  Returns new blocks dragged onto the workspace.
         *  @param the unique ID for the block requested.
         *  @return a new block matching the input ID.
         */
        virtual juckly::Block* doMakeBlock (String& blockID) override
        {
            InfoBar::get().updateContents ("Added " + blockID + "!");
            
            // Basic
            if (blockID == "StartBlock") return new StartBlock;
            
            if (blockID == "FourFourBar") return new FourFourBar;
            
            if (blockID == "ThreeFourBar") return new ThreeFourBar;
            
            if (blockID == "TwoFourBar") return new TwoFourBar;
            
            if (blockID == "FiveFourBar") return new FiveFourBar;
            
            // Instruments
            if (blockID == "Piano") return new Piano;
            if (blockID == "Glockenspiel") return new Glockenspiel;
            if (blockID == "AcousticGuitar") return new AcousticGuitar;
            if (blockID == "DistortionGuitar") return new DistortionGuitar;
            if (blockID == "ElectricBass") return new ElectricBass;
            if (blockID == "Violin") return new Violin;
            if (blockID == "Viola") return new Viola;
            if (blockID == "Cello") return new Cello;
            if (blockID == "Trumpet") return new Trumpet;
            if (blockID == "AltoSax") return new AltoSax;
            if (blockID == "TenorSax") return new TenorSax;
            if (blockID == "Bassoon") return new Bassoon;
            if (blockID == "Flute") return new Flute;
            if (blockID == "Pad") return new Pad;
            
            // Time
            if (blockID == "StartRepeat") return new StartRepeat;
            
            if (blockID == "EndRepeat") return new EndRepeat;
            
            if (blockID == "TempoSetter") return new TempoSetter;
                        
            if (blockID == "TempoChanger") return new TempoChanger;

            
            // Pitch
            if (blockID == "TrebleClef") return new TrebleClef;
            
            if (blockID == "BassClef") return new BassClef;
            
            // Dynamics
            if (blockID == "DynamicsBlock") return new DynamicsBlock;
            
            if (blockID == "DynamicsChanger") return new DynamicsChanger;

            
            // If you've broken here the block you have tried to create
            // does not exist!
            jassert (false);
            InfoBar::get().updateContents();
            return nullptr; 
        }
    };
    
} // namespace codetta
