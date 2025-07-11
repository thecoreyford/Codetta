/*
  ==============================================================================

    SingleSampleSynth.cpp
    Created: 16 Apr 2019 7:21:50pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "SingleSampleSynth.h"

/** Main namespace for codetta. */
namespace codetta
{
    SingleSampleSynth::SingleSampleSynth (const void* sourceData,
                                          size_t sourceDataSize,
                                          const int maxVoices,
                                          const int midiPitch)
    {
        setNoteStealingEnabled (true);
        
        for (int i = 0; i < maxVoices; ++i)
            addVoice (new SamplerVoice());
        
        // setup format manager to deal with the WAV samples
        audioFormatManager.registerBasicFormats();
        
        // read the wav file for the sampler sound object
        std::unique_ptr<AudioFormatReader> reader;
        
        
        reader.reset (audioFormatManager.createReaderFor (std::make_unique<MemoryInputStream> (sourceData,
                                                  sourceDataSize,
                                                  false)));
        
        // calculate the duration of the sample
        auto sampleDuration = reader->lengthInSamples / reader->sampleRate;
        
        // allow sound to be played on all notes
        BigInteger allNotes;
        allNotes.setRange (0, 128, true);
        
        //        (const String& name,
        //         AudioFormatReader& source,
        //         const BigInteger& midiNotes,
        //         int midiNoteForNormalPitch,
        //         double attackTimeSecs,
        //         double releaseTimeSecs,
        //         double maxSampleLengthSeconds);
        
        // add the sound to the synth
        addSound (new SamplerSound ("default", *reader, allNotes, midiPitch, 0, 0.2, sampleDuration));
    }
    
} // namespace codetta
