/*
  ==============================================================================

    FragmentFactory.h
    Created: 5 Oct 2018 7:06:31pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "NoteFragment.h"
#include "DoubleQuaver.h"
#include "BasicNoteFragments.h"

/** Namespace for the MusiSync Notation Engine */
namespace MusiSyncEng
{
    /**
     *  Enumeration of different fragment options avaliable.
     */
    enum Fragment
    {
//        doubleQuaver,
        semibreve,
        minim,
        crotchet,
        quaver,
//        semiquaver,
        totalFragments
    };
    
    //==========================================================================
    
    /**
     *  Factory for creating different fragment options.
     */
    class FragmentFactory
    {
    public:
        /**
         *  Creator function for note fragment objects.
         *  @param the fragment type to be created.
         *  @return a unique ptr to a fragment object.
         */
        std::unique_ptr<NoteFragment> createNoteFragment (Fragment fragment)
        {
            // Complex Fragments
//            if (fragment == doubleQuaver) return std::make_unique<DoubleQuaver>();
            
            // Basic Fragments
            if (fragment == semibreve) return std::make_unique<Semibreve>();
            
            if (fragment == minim) return std::make_unique<Minim>();
            
            if (fragment == crotchet) return std::make_unique<Crotchet>();
            
            if (fragment == quaver) return std::make_unique<Quaver>();
    
//            if (fragment == semiquaver) return std::make_unique<Semiquaver>();

            // The fragment selected hasn't been returned.
            return nullptr;
            
            
        }
    };
    
} // namespace MusiSyncEng
