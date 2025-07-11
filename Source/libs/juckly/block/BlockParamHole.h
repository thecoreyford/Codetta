/*
  ==============================================================================

    BlockParamHole.h
    Created: 4 Sep 2018 3:36:26pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once
#include "BlockManipulator.h"
#include "../../../../JuceLibraryCode/JuceHeader.h"

//==============================================================================

/** Main namespace for the juckly library */
namespace juckly
{
    /**
     *  Component that accepts function blocks, creating a parameter hole.
     */
    class BlockParamHole    : public Component
    {
    public:
        /** Constructor.*/
        BlockParamHole();
        
        /** Destructor */
        ~BlockParamHole();
 
        /**
         *  Sets the internal param to the value parsed. 
         *  @param the new component to act as the parameter.
         */
        void addInternalParam (Component* newInternalParam);

        /**
         *  Returns the internal parameter component. 
         *  A dynamic_cast can be used to access its values: as in this case 
         *    we can gaurantee its type.
         *  @return ponter to the current internal object.
         */
        Component* getInternalParam();

        /**
         *  Painting for the parameter component.
         *  @param the graphics context.
         */
        void paint (Graphics& g) override;

        /**
         *  Applys size changes if object size varies.
         */
        void resized() override;
        
    private:
        /** The internal object acting as this blocks parameter value */
        Component* internalParam;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlockParamHole)
    };
    
} // namespace juckly
