/*
  ==============================================================================

    LicenseInfo.cpp
    Created: 29 May 2019 5:10:15pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "LicenseInfo.h"

/** Main namespace for codetta. */
namespace codetta
{
    LicenseInfo::LicenseInfo(String webpageRhs)    : webpage(webpageRhs)
    {
        // setup box size
        auto w = Desktop::getInstance().getDisplays().getMainDisplay().totalArea.getWidth();
        auto h = Desktop::getInstance().getDisplays().getMainDisplay().totalArea.getHeight();
        setSize (w * 0.5, h * 0.5);
        
//        // fill text edit with license info
//        String MITLicense = CharPointer_UTF8 ("Copyright (c) S. Christian Collins Copyright \xc2\xa9 2018\n"
//                                              "\n"
//                                              "Mono version: Copyright (c) 2014-16 Michael Cowgill Copyright (c) 2000-2002, 2008 Frank Wen getfrank"
//                                              "@gmail.com\n"
//                                              "\n"
//                                              "Permission is hereby granted, free of charge, to any person obtaining a copy of this software and as"
//                                              "sociated documentation files (the \"Software\"), to deal in the Software without restriction, includin"
//                                              "g without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or"
//                                              " sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subj"
//                                              "ect to the following conditions:\n"
//                                              "\n"
//                                              "The above copyright notice and this permission notice shall be included in all copies or substantial"
//                                              " portions of the Software.\n"
//                                              "\n"
//                                              "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NO"
//                                              "T LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT"
//                                              ". IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABI"
//                                              "LITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WIT"
//                                              "H THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
//        textEdit.setScrollbarsShown(true);
//        textEdit.setReadOnly(true);
//        textEdit.setMultiLine(true);
//        textEdit.setColour (TextEditor::ColourIds::backgroundColourId,
//                            Colours::white);
//        textEdit.setColour (TextEditor::ColourIds::textColourId,
//                            Colours::black);
//        textEdit.setText (MITLicense);
//        addAndMakeVisible (textEdit);
        
        addAndMakeVisible(browser);
        browser.goToURL(webpage);
    }
    
    LicenseInfo::~LicenseInfo()
    {
        
    }
    
    void LicenseInfo::paint (Graphics& g)
    {
        g.fillAll (Colours::lightgrey);
//
//        auto topInfo = getLocalBounds().withTrimmedBottom (getHeight() * 0.80);
//        auto centreInfo = getLocalBounds().withTrimmedTop (getHeight() * 0.20).withTrimmedBottom(getHeight() * 0.20);
//        auto bottomInfo = getLocalBounds().withTrimmedTop (getHeight() * 0.80);
//
//        String intro = CharPointer_UTF8 ("Codetta\'s sounds are provided via the MS_General.sf2 by S. Christian Collins Copyright \xc2\xa9 2018, under"
//                                        " the MIT license described below.");
//        g.drawFittedText (intro, topInfo, Justification::centred, 2);
//
//        textEdit.setBounds (centreInfo);
//
//        String outro = "For full details vist:\n https://github.com/musescore/MuseScore/blob/master/share/sound/FluidR3Mono_License.md";
//        g.drawFittedText(outro, bottomInfo, Justification::centred, 2);
    }
    
    void LicenseInfo::resized()
    {
        browser.setBounds(getLocalBounds());
    }
 
} // namespace codetta

