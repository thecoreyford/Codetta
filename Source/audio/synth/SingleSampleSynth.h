/*
  ==============================================================================

    SingleSampleSynth.h
    Created: 16 Apr 2019 7:21:50pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once
#include "../../../JuceLibraryCode/JuceHeader.h"

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Synthesiser with a single sample voicing.
     */
    class SingleSampleSynth    : public Synthesiser
    {
    public:
        /** Constructor.
         *  @param sourceData is the Binary audio Data for the sample.
         *  @param sourceDataSize is the size of the sample.
         *  @param maxVoices is the number of voices this synth uses.
         */
        SingleSampleSynth (const void* sourceData,
                           size_t sourceDataSize,
                           const int maxVoices = 64,
                           const int midiPitch = 60);
        
    private:
        
        /** Private constructor. */
        SingleSampleSynth();
        
        /** Object dealing with audio formats (wav etc.)*/
        AudioFormatManager audioFormatManager;
    };
} // namespace codetta
