/*
  ==============================================================================

    Audio.cpp
    Created: 11 Jan 2019 3:32:25pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "Audio.h"

/** Main namespace for codetta. */
namespace codetta
{
    Audio::Audio()
    {
        audioDeviceManager.initialiseWithDefaultDevices (0 /*inputs*/,2 /*outputs*/);
     
        setupSoundfont();
        // load the soundfont into the audio source
        audioSourcePlayer.setSource (&soundfontAudioSource);
        
        // add the audio callback
        audioDeviceManager.addAudioCallback (this);
    }
    
    Audio::~Audio()
    {
        audioSourcePlayer.setSource (nullptr);
        audioDeviceManager.removeAudioCallback (this);
    }
    
    //======================================================================

    AudioDeviceManager& Audio::getAudioDeviceManager()
    {
        return audioDeviceManager;
        
    }
    
    MidiOut& Audio::getMidiOutput()
    {
        return mo;
    }
    
    //==========================================================================

    void Audio::audioDeviceAboutToStart (AudioIODevice* device)
    {
        audioSourcePlayer.audioDeviceAboutToStart (device);
    }
    
    void Audio::audioDeviceIOCallback (const float** inputChannelData,
                                       int numInputChannels,
                                       float** outputChannelData,
                                       int numOutputChannels,
                                       int numSamples)
    {
        audioSourcePlayer.audioDeviceIOCallback (inputChannelData,
                                                 numInputChannels,
                                                 outputChannelData,
                                                 numOutputChannels,
                                                 numSamples);
    }
    
    void Audio::audioDeviceStopped()
    {
        audioSourcePlayer.audioDeviceStopped();
    }
    
    //==========================================================================
    
    void Audio::setupSoundfont()
    {
        // Load soundfont
        String soundfontFilePath (File::getSpecialLocation (File::SpecialLocationType::currentApplicationFile).getFullPathName());
        soundfontFilePath += "/Contents/Resources/GMSoundFont.sf2";
        const File soundfontFile (soundfontFilePath);
        soundfontAudioSource.loadSoundfont (soundfontFile);
        
        // Set channels with correct program change messages
        setProgramChangeMessagesForChannels();
    }
    
    void Audio::setProgramChangeMessagesForChannels()
    {
        int channel = 0;
        for (auto GMInstrument : {0, 9, 24, 29, 33, 40, 41, 42, 56, -1, 65, 66, 70, 73, 81})
        {
            channel++;
            if (channel != 10) // as it is reserved for percussion sounds
                soundfontAudioSource.programChange (channel, GMInstrument);
        }
    }

} // namespace codetta
