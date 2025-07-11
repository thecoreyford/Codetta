/*
  ==============================================================================

    UndoWidget.h
    Created: 16 Mar 2021 4:22:51pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include <deque>

//==============================================================================
/** Main namespace for codetta. */
namespace codetta
{
    //TODO: comment this class

    class UndoWidget    : public ImageButton
    {
    public:
        static UndoWidget& get()
        {
            static UndoWidget instance;
            return instance;
        }
        
        ~UndoWidget();
        
        class Listener
        {
        public:
            virtual ~Listener(){}
            virtual XmlElement onUndoCommand(bool update, XmlElement elm) = 0;
        };
        
        void addListener(Listener* listenerRhs);
        
        void updateUndoStack();
        
        void rollback();

        void clear();
        
        bool isUndoing();
        
        void setIsUndoing(bool state);
    private:
        
        UndoWidget();

        std::deque<XmlElement> snapshots;
        
        Listener* listener;
        
        bool currentlyUndoing = false;
        
        const int bufferSize = 30;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UndoWidget)
    };

}  //namespace codetta
