/*
  ==============================================================================

    ContextTracker.h
    Created: 28 Dec 2020 3:30:33pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"

//==============================================================================

/** Main namespace for codetta */
namespace codetta
{
    /**
     *  Keeps references to all the notes and bars that are added, and also creates context dependent sound.
     */
    class ContextTracker
    {
    public:
        
        /**
         * Singleton getter for the clipboard.
         * @returns instance of the clipboard.
         */
        static ContextTracker& get()
        {
            static ContextTracker instance;
            return instance;
        }
        
        /** Destructor */
        ~ContextTracker() {}
        
        /**
         * Directs block ID's to main, in order to display the relevant help!
         */
        void showHelpFor(const String& help)
        {
            requestedHelp = help;
            if (helpTrigger != nullptr){
                helpTrigger->resized();
            }
            requestedHelp = "none";
        }
        
        void setHelpTrigger(Component* helpTrig)
        {
            helpTrigger = helpTrig;
        }
        
        const String& getRequestedHelp() { return requestedHelp; }
        
        //=========================================================
        
        void updateLastUsed (String ID, Component* comp)
        {
            lastUsed[ID] = comp;
            
            if (listener != nullptr && ID == "Note")
            {
                listener->onContext (comp);
            }
        }
        
        Component* getLastUsed (String ID) { return lastUsed[ID]; }
        
        void cleanLastUsed()
        {
            for (auto& [_, v] : lastUsed) v = nullptr;
        }
        
        class Listener
        {
        public:
            virtual ~Listener(){}
            virtual void onContext (Component* comp) = 0;
        };

        void setListener (Listener* newListener) {listener = newListener;}
        
        
        
    private:
        /** Private constructor. */
        ContextTracker() {}
        
        String requestedHelp = "none";
        
        std::map<String, Component*> lastUsed;
        
        Component* helpTrigger = nullptr;
        
        Listener* listener = nullptr;
    };
} // namespace codetta
