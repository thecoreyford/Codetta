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
        
        // load the custom audio sourceinto the player
        audioSourcePlayer.setSource (&codettaAudioSource);
        
        // add the audio callback
//        audioDeviceManager.addAudioCallback (this);
        audioDeviceManager.addAudioCallback(&audioSourcePlayer);
    }
    
    Audio::~Audio()
    {
        audioSourcePlayer.setSource (nullptr);
//        audioDeviceManager.removeAudioCallback (this);
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

//    void Audio::audioDeviceAboutToStart (AudioIODevice* device)
//    {
//        audioSourcePlayer.audioDeviceAboutToStart (device);
//        reverb.setSampleRate (device->getCurrentSampleRate());
//        
//        Reverb::Parameters params = reverb.getParameters();
//        params.wetLevel = 0.8f;
//        params.damping = 0.5f;
//        params.width = 0.8f;
//        params.dryLevel = 0.6f;
//        params.roomSize = 0.3f;
//        reverb.setParameters (params);
//    }
//    
//    void Audio::audioDeviceIOCallback (const float** inputChannelData,
//                                       int numInputChannels,
//                                       float** outputChannelData,
//                                       int numOutputChannels,
//                                       int numSamples)
//    {
////        audioSourcePlayer.audioDeviceIOCallback (inputChannelData,
////                                                 numInputChannels,
////                                                 outputChannelData,
////                                                 numOutputChannels,
////                                                 numSamples);
//
//        auto outLeft = outputChannelData[0];
//        auto outRight = outputChannelData[1];
//        const float outGain = 0.010;
//        
//        // Buffer rate block...
//        if (outRight == nullptr) // iphones and the like are mono
//            reverb.processMono (outLeft, numSamples);
//        else
//            reverb.processStereo (outLeft, outRight, numSamples);
//
//
//        // Sample rate block...
//        while (numSamples--)
//        {
//            *outLeft *= outGain;
//            outLeft++;
//            
//            if (outRight != nullptr) // iphones and the like are mono
//            {
//                *outRight *= outGain;
//                outRight++;
//            }
//        }
//        
//    }
//    
//    void Audio::audioDeviceStopped()
//    {
//        reverb.reset();
//        audioSourcePlayer.audioDeviceStopped();
//    }
    
} // namespace codetta
