/*
  ==============================================================================

    LicenseInfo.h
    Created: 29 May 2019 5:10:15pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Component containing information for the MIT License Soundfont used!
     */
    class LicenseInfo    : public Component
    {
    public:
        /** Constructor */
        LicenseInfo(String webpage);
        
        /** Destructor */
        ~LicenseInfo();
        
        /**
         *  Paint method.
         *  @param the graphics context for this object.
         */
        void paint (Graphics&) override;
        
        /** Resized function. */
        void resized() override;
        
    private:
        /** Text edit displaying license info.*/
//        TextEditor textEdit;
        
        /** Component to load tutorial webpages. */
        WebBrowserComponent browser;
        
        String webpage;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LicenseInfo)
    };
    
} // namespace codetta
