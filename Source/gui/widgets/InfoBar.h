/*
  ==============================================================================

    InfoBar.h
    Created: 11 Jan 2019 12:22:26pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../libs/MusicSync-Notation-Engine/Font.h"
#include "../../libs/MusicSync-Notation-Engine/OffsetMappings.h"

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Widget displaying the info it recives.
     */
    class InfoBar    : public Component
    {
    public:
        
        /**
         *  Acessor function for the InfoBar singleton.
         *  @return reference to the Infobar class.
         */
        static InfoBar& get()
        {
            static InfoBar instance;
            return instance;
        }
        
        /**
         *  Paint callback.
         *  @param the graphics context for this object.
         */
        void paint (Graphics& g) override;
        
        
        /** Updates the visible message to its default message. */
        void updateContents();
        
        /**
         *  Updates the visible message.
         *  @param the new message to be written.
         */
        void updateContents (const String& message);

        /**
         *  Updates the visible message for a note.
         *  @param the notes offset mappping.
         */
        void updateContentForNote (int offsetMapping,
                                   MusiSyncEng::NewFont::NoteValue value);
        
    private:
        
        //======================================================================
        
        /** Private Constructor. */
        InfoBar() { displayedMessage = "Codetta"; }
        
        /** Private Destructor */
        ~InfoBar() {}

        /**
         *  Private Copy Constructor
         *  @param copyable value
         */
        InfoBar (const InfoBar&){}
        
        /**
         *  Private Copy Operator
         *  @param copyable value
         */
        void operator= (const InfoBar&){}
        
        //======================================================================
        
        /** The message curretly visible.*/
        String displayedMessage;
    };
} // namespace codetta
    
