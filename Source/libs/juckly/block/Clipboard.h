/*
  ==============================================================================

    Clipboard.h
    Created: 20 Dec 2019 1:53:34pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../../JuceLibraryCode/JuceHeader.h"

//==============================================================================

#define CODETTA 0 //< add macro so that the libs folder can remain clean for
                  // potentially releasing as an API.
#ifdef CODETTA
    #include "../../../gui/widgets/InfoBar.h"
    #include "../../../data-logger/DataLogger.h"
#endif

//==============================================================================

/** Main namespace for the juckly library */
namespace juckly
{
    /**
     *  Clipboard object -  XML element description to the clipboard that can used to paste items.
     */
    class Clipboard
    {
    public:
        
        /**
         * Singleton getter for the clipboard.
         * @returns instance of the clipboard.
         */
        static Clipboard& get()
        {
            static Clipboard instance;
            return instance;
        }
        
        /** Destructor */
        ~Clipboard() {}
        
        /**
         * Writes the xml description to be copied.
         * @param an xml desription for the block (or other) to be copied.
         */
        void copy (XmlElement* itemToBeCopied)
        {
            #ifdef CODETTA
            codetta::InfoBar::get().updateContents ("Block "
                                                    + itemToBeCopied->getStringAttribute("id")
                                                    + " copied!");
            LOG_STRING ("Block " + itemToBeCopied->getStringAttribute("id") + " copied");
            #endif
        
            copiedItem.reset (itemToBeCopied);
        }
        
        /**
         * Returns the description to be pasted (getter).
         * @return the xml element description to be pasted.
         */
        std::unique_ptr<XmlElement>& paste()
        {
                return copiedItem;
        }
        
        /**
         * Sets the co-ordinates where blocks should be pasted (for Async)
         */
        void setPasteCoords(const Point<int>& points){
            pasteCoords = points;
        }
        
        /**
        * Returns the points where block should be pasted.
        * @return the points for where the block should be pasted.
        */
        const Point<int>& getPasteCoords(){
            return pasteCoords;
        }
        
     
        
    private:
        /** Private constructor. */
        Clipboard() { copiedItem = nullptr; }
        
        /** Block Co-Ordinates */
        Point<int> pasteCoords;
        
        /** Current description to be copied. */
        std::unique_ptr<XmlElement> copiedItem;
    };
} // namespace juckly
