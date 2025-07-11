/*
  ==============================================================================

    Audio.h
    Created: 22 Oct 2018 11:45:18pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../3rdparty/juce-soundfonts/Source/SoundfontAudioSource.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiEventList.h"
#include "MidiOut.h"
#include "PlaybackSettings.h"

/** Main namespace for codetta. */
namespace codetta
{
    /**
     *  Class containing all audio processes.
     */
    class Audio :   public AudioIODeviceCallback
    {
    public:
        
        /** Constructor */
        Audio();
        
        /** Destructor */
        ~Audio();
        
        //======================================================================
        
        /**
         *  Getter for the audio device manager.
         *  @return the audio device manager.
         */
        AudioDeviceManager& getAudioDeviceManager();
        
        /**
         *  Getter for the midi output device.
         *  @return the midi output class.
         */
        MidiOut& getMidiOutput();
    
        //======================================================================

        /**
         *  Callback for each audio buffer.
         *  @param inputChannel data is the samples flowing into our buffer.
         *  @param numInputChannels is the number of avaliable audio channels input.
         *  @param outputChannelData is the samples to leave from the callback.
         *  @param numOutputChannels is the number of remaing audio channels.
         *  @param numSamples is the number of samples contained within each buffer.
         */
        void audioDeviceIOCallback (const float** inputChannelData,
                                    int numInputChannels,
                                    float** outputChannelData,
                                    int numOutputChannels,
                                    int numSamples) override;
        
        /**
         *  Called before the audioDeviceIOCallback, for intialisation.
         *  @param the audioIODevice intialised.
         */
        void audioDeviceAboutToStart (AudioIODevice* device) override;
        
        /** Called once the audioDeviceIOCallback has finished, for releasing. */
        void audioDeviceStopped() override;
        
        //======================================================================
        
        /** Initialise wrapper for all sound font functions. */
        void setupSoundfont();
        
    private:
        /** Assigns the instruments to the corresponding channels. @see PlaybackSettings*/
        void setProgramChangeMessagesForChannels();
        
        //======================================================================
        // Private Copy constructors & Assignment overloading.
        // You should only have a single instance audio within the program,
        // mixing various sources if needed!
        
        /**
         *  Private copy constructor. See header file.
         *  @param object to be copied.
         */
        Audio (const Audio& rhs);
        
        /**
         *  Private copy operator. See header file.
         *  @param object to be copied.
         */
        Audio operator = (const Audio& rhs);
        
        //======================================================================
        /** Device manager, interfacing with the speaker output. */
        AudioDeviceManager audioDeviceManager;
        
        /** Player for our audio source objects. */
        AudioSourcePlayer audioSourcePlayer;
        
        //======================================================================
        
        /** Soundfont audio source playback */
        SoundfontAudioSource soundfontAudioSource;
        
        /** Midi output */
        MidiOut mo {soundfontAudioSource};
    };
} // namespace codetta


