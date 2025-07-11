/*
  ==============================================================================

    CodettaAudioSource.cpp
    Created: 16 Apr 2019 7:41:02pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "CodettaAudioSource.h"

/** Main namespace for codetta. */
namespace codetta
{
    CodettaAudioSource::CodettaAudioSource()
    {
        setCurrentInstrumentForChannel (1);
        
        // add each instrument in the order seen in @see PlaybackSettings.h Instrument enum
        // to match the channels.
        
        // channel 1
        synth.push_back (std::make_unique<SingleSampleSynth>(BinaryData::piano_wav,
                                                             BinaryData::piano_wavSize));
        
        // channel 2 - TODO: get legit glock sample
        synth.push_back (std::make_unique<SingleSampleSynth>(BinaryData::glock_wav,
                                                             BinaryData::glock_wavSize));
        
        // channel 3
        synth.push_back (std::make_unique<SingleSampleSynth>(BinaryData::noise_wav,
                                                             BinaryData::noise_wavSize));
        
        // channel 4
        synth.push_back (std::make_unique<SingleSampleSynth>(BinaryData::noise_wav,
                                                             BinaryData::noise_wavSize));
        
        
        // channel 5
        synth.push_back (std::make_unique<SingleSampleSynth>(BinaryData::noise_wav,
                                                             BinaryData::noise_wavSize));
        
        
        // channel 6
        synth.push_back (std::make_unique<SingleSampleSynth>(BinaryData::violin_wav,
                                                             BinaryData::violin_wavSize));
        
        // channel 7
        synth.push_back (std::make_unique<SingleSampleSynth>(BinaryData::viola_wav,
                                                             BinaryData::viola_wavSize));
        
        // channel 8
        synth.push_back (std::make_unique<SingleSampleSynth>(BinaryData::cello_wav,
                                                             BinaryData::cello_wavSize));
        
        // channel 9
        synth.push_back (std::make_unique<SingleSampleSynth>(BinaryData::trumpet_wav,
                                                             BinaryData::trumpet_wavSize));
        
        // channel 10 //TODO: THIS SHOULD BE DRUMS!
        synth.push_back (std::make_unique<SingleSampleSynth>(BinaryData::noise_wav,
                                                             BinaryData::noise_wavSize));
        // channel 11
        synth.push_back (std::make_unique<SingleSampleSynth>(BinaryData::noise_wav,
                                                             BinaryData::noise_wavSize));
        
        // channel 12
        synth.push_back (std::make_unique<SingleSampleSynth>(BinaryData::noise_wav,
                                                             BinaryData::noise_wavSize));
        
        // channel 13
        synth.push_back (std::make_unique<SingleSampleSynth>(BinaryData::noise_wav,
                                                             BinaryData::noise_wavSize));
        
        // channel 14
        synth.push_back (std::make_unique<SingleSampleSynth>(BinaryData::noise_wav,
                                                             BinaryData::noise_wavSize));
        
        // channel 15
        synth.push_back (std::make_unique<SingleSampleSynth>(BinaryData::pad_wav,
                                                             BinaryData::pad_wavSize));
    }
    
    CodettaAudioSource::~CodettaAudioSource()
    {
    }
    
    //==========================================================================
    
    void CodettaAudioSource::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
    {
        for (int i = 0; i < synth.size(); ++i)
            synth[i]->setCurrentPlaybackSampleRate (sampleRate);
    }
    
    
    void CodettaAudioSource::releaseResources(){}
    
    void CodettaAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
    {
        bufferToFill.clearActiveBufferRegion();
        const MidiBuffer inputMidi;
        
        // Mix the buffers for all synths
        for (int i = 0; i < synth.size(); ++i)
            synth[i]->renderNextBlock (*bufferToFill.buffer,
                                       inputMidi,
                                       bufferToFill.startSample,
                                       bufferToFill.numSamples);
    }
    
    //==========================================================================
    
    void CodettaAudioSource::noteOn (int note, int velocity, int channel)
    {
        synth[currentInstrument - 1]->noteOn (channel, note, velocity);
    }
    
    void CodettaAudioSource::noteOff (int note, int channel)
    {
        synth[currentInstrument - 1]->noteOff (channel, note, 0.0f, true);
    }
    
    void CodettaAudioSource::allNotesOff (int channel)
    {
        synth[currentInstrument - 1]->allNotesOff (channel, false);
    }
    
    //==========================================================================

    void CodettaAudioSource::processMIDI (const MidiMessage& message)
    {
        setCurrentInstrumentForChannel (message.getChannel());
        
//        // useful for debugging at message level
//        DBG (message.getTimeStamp() << " - " << message.getNoteNumber() <<
//              (message.isNoteOn() ? " - on" : " - off"));
        
        if (message.isNoteOn())     noteOn (message.getNoteNumber(),
                                            message.getVelocity(),
                                            message.getChannel());
        
        if (message.isNoteOff())    noteOff (message.getNoteNumber(),
                                             message.getChannel());
        
        if (message.isAllNotesOff()) allNotesOff(message.getChannel());
    }
    
    //==========================================================================
    
    void CodettaAudioSource::setCurrentInstrumentForChannel (int channel)
    {
        // You cannot have a "zeroth" channel (1-16)!!!
        jassert (channel > 0);
        
        // There are not this many instruments loaded into Codetta.
        jassert (channel < Instrument::total);
        
        // Set instument to minus one (to match the vector)
        currentInstrument = static_cast<Instrument>(channel);
    }
    
} // namespace codetta
