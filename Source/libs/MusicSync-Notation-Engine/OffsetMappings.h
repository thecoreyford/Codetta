/*
  ==============================================================================

    OffsetMappings.h
    Created: 21 Oct 2018 1:23:26pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"

/** Namespace for the MusiSync Notation Engine */
namespace MusiSyncEng
{
    /**
     *  Enumeration for different supported clef types.
     */
    enum ClefTypes
    {
        Treble,
        Bass,
        Alto,
        Tenor
    };
    
    //==============================================================================
    
    // REF: (Gaster, 2018)
    /**
     *  Enumeration for MidiNote values to their note symbols.
     */
    enum NoteSym
    {
        A0  = 21,
        AS0 = 22,
        B0  = 23,
        
        C1  = 24,
        CS1 = 25,
        D1  = 26,
        DS1 = 27,
        E1  = 28,
        F1  = 29,
        FS1 = 30,
        G1  = 31,
        GS1 = 32,
        A1  = 33,
        AS1 = 34,
        B1  = 35,
        
        C2  = 36,
        CS2 = 37,
        D2  = 38,
        DS2 = 39,
        E2  = 40,
        F2  = 41,
        FS2 = 42,
        G2  = 43,
        GS2 = 44,
        A2  = 45,
        AS2 = 46,
        B2  = 47,
        
        C3  = 48,
        CS3 = 49,
        D3  = 50,
        DS3 = 51,
        E3  = 52,
        F3  = 53,
        FS3 = 54,
        G3  = 55,
        GS3 = 56,
        A3  = 57,
        AS3 = 58,
        B3  = 59,
        
        C4  = 60, //< middle C
        CS4 = 61,
        D4  = 62,
        DS4 = 63,
        E4  = 64,
        F4  = 65,
        FS4 = 66,
        G4  = 67,
        GS4 = 68,
        A4  = 69,
        AS4 = 70,
        B4  = 71,
        
        C5  = 72,
        CS5 = 73,
        D5  = 74,
        DS5 = 75,
        E5  = 76,
        F5  = 77,
        FS5 = 78,
        G5  = 79,
        GS5 = 80,
        A5  = 81,
        AS5 = 82,
        B5  = 83,
        
        C6  = 84,
        CS6 = 85,
        D6  = 86,
        DS6 = 87,
        E6  = 88,
        F6  = 89,
        FS6 = 90,
        G6  = 91,
        GS6 = 92,
        A6  = 93,
        AS6 = 94,
        B6  = 95,
        
        C7  = 96,
        CS7 = 97,
        D7  = 98,
        DS7 = 99,
        E7  = 100,
        F7  = 101,
        FS7 = 102,
        G7  = 103,
        GS7 = 104,
        A7  = 105,
        AS7 = 106,
        B7  = 107,
        
        C8  = 108
    };
    
    //==============================================================================
    
    /**
     *  Class mapping note offsets to their musical values.
     */
    class OffsetMappings
    {
    public:
        /** Constructor */
        OffsetMappings ()
        {
            // Treble clef by default...
            setMappingToClef (ClefTypes::Treble);
        }
        
        /**
         *  Setter for clef values.
         *  @param the type of clef being used.
         */
        void setMappingToClef (ClefTypes clef)
        {
            // empty any previous mappings
            mapping.clear();
            
            if (clef == ClefTypes::Treble)
            {
                mapping.set (-3, NoteSym::C4);
                mapping.set (-2, NoteSym::D4);
                mapping.set (-1, NoteSym::E4);
                mapping.set (0, NoteSym::F4);
                mapping.set (1, NoteSym::G4);
                mapping.set (2, NoteSym::A4);
                mapping.set (3, NoteSym::B4);
                mapping.set (4, NoteSym::C5);
                mapping.set (5, NoteSym::D5);
                mapping.set (6, NoteSym::E5);
                mapping.set (7, NoteSym::F5);
                mapping.set (8, NoteSym::G5);
                mapping.set (9, NoteSym::A5);
                return;
            }
            
            if (clef == ClefTypes::Bass)
            {
                mapping.set (-3, NoteSym::E2);
                mapping.set (-2, NoteSym::F2);
                mapping.set (-1, NoteSym::G2);
                mapping.set (0, NoteSym::A2);
                mapping.set (1, NoteSym::B2);
                mapping.set (2, NoteSym::C3);
                mapping.set (3, NoteSym::D3);
                mapping.set (4, NoteSym::E3);
                mapping.set (5, NoteSym::F3);
                mapping.set (6, NoteSym::G3);
                mapping.set (7, NoteSym::A3);
                mapping.set (8, NoteSym::B3);
                mapping.set (9, NoteSym::C4);
                return;
            }
            
            //TODO: alto etc...
        }
        
        /**
         *  Getter for clef values.
         *  @return the hash map containing clef mapppings.
         */
        const HashMap<int, NoteSym>& getMapping() const
        {
            return mapping;
        }
        
        
        /**
         *  Getter for note symbol representation of any clef.
         *  @param The clef type requested.
         *  @param The integer value requested.
         *  @return The note symbol requested. 
         */
        const NoteSym getNoteSymForAnyClef (ClefTypes clef, int offset) const
        {
            if (clef == ClefTypes::Treble)
            {
                if (offset == -3) return NoteSym::C4;
                if (offset == -2) return NoteSym::D4;
                if (offset == -1) return NoteSym::E4;
                if (offset == 0) return NoteSym::F4;
                if (offset == 1) return NoteSym::G4;
                if (offset== 2) return NoteSym::A4;
                if (offset == 3) return NoteSym::B4;
                if (offset == 4) return NoteSym::C5;
                if (offset == 5) return NoteSym::D5;
                if (offset == 6) return NoteSym::E5;
                if (offset == 7) return NoteSym::F5;
                if (offset == 8) return NoteSym::G5;
                if (offset == 9) return NoteSym::A5;
            }
            
            if (clef == ClefTypes::Bass)
            {
                if (offset == -3) return NoteSym::E2;
                if (offset == -2) return NoteSym::F2;
                if (offset == -1) return NoteSym::G2;
                if (offset == 0) return NoteSym::A2;
                if (offset == 1) return NoteSym::B2;
                if (offset== 2) return NoteSym::C3;
                if (offset == 3) return NoteSym::D3;
                if (offset == 4) return NoteSym::E3;
                if (offset == 5) return NoteSym::F3;
                if (offset == 6) return NoteSym::G3;
                if (offset == 7) return NoteSym::A3;
                if (offset == 8) return NoteSym::B3;
                if (offset == 9) return NoteSym::C4;
            }
            return NoteSym::C4;
        }
        
    private:
        /** Hashmap holding clef values. */
        HashMap<int, NoteSym> mapping;
    };

} // namespace MusiSyncEng
