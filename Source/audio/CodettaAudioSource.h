/*
  ==============================================================================

    CodettaAudioSource.h
    Created: 16 Apr 2019 7:41:02pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "synth/SingleSampleSynth.h"
#include "PlaybackSettings.h"

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Custom audio source for Codetta's sounds.
     */
    class CodettaAudioSource    : public AudioSource
    {
    public:
        
        /** Alias for the instrument enum from @see PlaybackSettings */
        using Instrument = PlaybackSettings::Instrument;
        
        /** Constructor */
        CodettaAudioSource();
        
        /** Destructor */
        ~CodettaAudioSource();
        
        //======================================================================
        
        /**
         *  Initialisation function for audio source.
         *  @param samplesPerBlockExpected is the size of each block.
         *  @param sampleRate is the current sample rate of the audio device.
         */
        void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
        
        /** Shutdown function for the audio source. */
        void releaseResources() override;
        
        /**
         *  Getter for the next audio block. Sums synthesiser sounds together.
         *  @param buffer for this object.
         */
        void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
        
        //======================================================================
        
        /**
         *  Handles note on MIDI events.
         *  @param the MIDI note value.
         *  @param the MIDI velocity value.
         *  @param the channel number.
         */
        void noteOn (int note, int velocity, int channel = 1);
        
        /**
         *  Handles note off events.
         *  @param the MIDI note value.
         *  @param the channel number.
         */
        void noteOff (int note, int channel = 1);
        

        void allNotesOff (int channel = 1);
    
        //======================================================================
        
        /**
         *  Wrapper, handling any MIDI message passed.
         *  @param message to be played by this audio source.
         */
        void processMIDI (const MidiMessage& message);
        
    private:
        
        /**
         *  Setter for the current instrument, based on the channel.
         *  @param the MIDI channel. 
         */
        void setCurrentInstrumentForChannel (int channel);
        
        /** Vector holding all the single synth sounds for Codetta. */
        std::vector<std::unique_ptr<SingleSampleSynth>> synth;
        
        /** The current instrument for playback.*/
        Instrument currentInstrument;
    };
    
} // namespace codetta
