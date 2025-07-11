/*
  ==============================================================================

    Font.h
    Created: 2 Oct 2018 3:18:25pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"

/** Namespace for the MusiSync Notation Engine */
namespace MusiSyncEng
{
    /** Singleton of helper functions for using the MusiSync Font. */
    namespace NewFont
    {
        /**
         *  Getter for the notation font.
         *  @return reference to the font in binary data.
         */
        static const Font& getNotationFont()
        {
            static Font notation (Font (Typeface::createSystemTypefaceFor (BinaryData::MusiSync_ttf,
                                                                           BinaryData::MusiSync_ttfSize)));
            return notation;
        }
        
        //=================================================================================================
        
        /** Enumeration of note values avaliable in the font. */
        enum NoteValue
        {
            semibreve,
            minim,
            crotchet,
            quaver,
//            semiquaver,
            totalNoteValue
        };
        
        /**
         *  Getter for the notes division for each note value.
         *  @param The note value requested.
         *  @return the numeric division for that note value.
         */
        static float getNoteDivision (NoteValue noteValue)
        {
            switch (static_cast<int>(noteValue))
            {
                case NoteValue::semibreve : return 1.0f; break;
                case NoteValue::minim : return 0.5f; break;
                case NoteValue::crotchet : return 0.25f; break;
                case NoteValue::quaver : return 0.125f; break;
//                case NoteValue::semiquaver : return 0.0625f; break;
            }
            // Note value was not found.
            jassert (false);
            return -1.0f;
        }
        
        /**
         *  Getter for the character for each note value.
         *  @param The note value requested.
         *  @return the ascii representation for that note value.
         */
        static char getNoteAsChar (NoteValue noteValue)
        {
            switch (static_cast<int>(noteValue))
            {
                case NoteValue::semibreve : return 'w'; break;
                case NoteValue::minim : return 'h'; break;
                case NoteValue::crotchet : return 'q'; break;
                case NoteValue::quaver : return 'e'; break;
//                case NoteValue::semiquaver : return 's';  break;
            }
            
            // Note value was not found.
            jassert (false);
            
            return '!';
        }
        
        /**
         *  Getter for the string for each note value.
         *  @param The note value requested.
         *  @return the string representation for each note value.
         */
        static String getNoteAsText (NoteValue noteValue)
        {
            switch (static_cast<int>(noteValue))
            {
                case NoteValue::semibreve : return "semibreve"; break;
                case NoteValue::minim : return "minim"; break;
                case NoteValue::crotchet : return "crotchet"; break;
                case NoteValue::quaver : return "quaver"; break;
                    //                case NoteValue::semiquaver : return 's';  break;
            }
            
            // Note value was not found.
            jassert (false);
            
            return String();
        }
        //======================================================================
        
    } // namespace NewFont
} // namespace MusicSyncEng
