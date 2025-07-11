/*
  ==============================================================================

    AmysBarBlocks.h
    Created: 17 Jun 2021 10:54:15am
    Author:  Corey Ford

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "../libs/juckly/client/Block.h"
#include "../libs/Amys-Notation/BlockComponent.h"
#include "../libs/Amys-Notation/DrawComponent.h"

#pragma once

/** Main namespace for codetta. */
namespace codetta
{
    /** Namespace for the notation created by Amy */
    namespace amy
    {
        class PianoRollBar    : public juckly::Block
        {
        public:
            
            /** Default constructor. */
            PianoRollBar () : juckly::Block (std::make_unique<juckly::CommandBlockSettings>
                                                ("PianoRollBar",
                                                   ImageCache::getFromMemory(BinaryData::pianoRollBlockIcon_png, BinaryData::pianoRollBlockIcon_pngSize),
                                                   ImageCache::getFromMemory (BinaryData::fourFourBlock_png,  BinaryData::fourFourBlock_pngSize),
                                                   false /* not a start node */,
                                                   new BlockComponent(),
                                                   false /* dosen't take a parameter */,
                                                   352
                                                )
                                            )
            {
                // scale connections to fit the image used
                getScaling().setConnectionWidthScaling (0.045);
            }
            
            /** Calculates the timing for a bar. */
            void doAction() override
            {
                auto bar = dynamic_cast<BlockComponent*>(getInternalUI());
                
                //========================================================================
                // Get the things from Amy's bar...
                juce::Array<int> noteDurationOut;
                juce::Array<int> notePitchOut;
                juce::Array<int> visibleColumnIndicesOut;
                
                bar->getAudioInfo(noteDurationOut, notePitchOut, visibleColumnIndicesOut);
                
                //========================================================================
                
                //========================================================================
                // Scale to values for Codetta...
                juce::Array<int> notePitches;
                juce::Array<float> noteDurations;
                
                for (int i = 0; i < notePitchOut.size(); ++i)
                {
                    float scaledPitch = notePitchOut[i] + 1;
                    HashMap<int, int> pitchMapping;
                    

                    if (PlaybackSettings::get().getCurrentClef() == MusiSyncEng::ClefTypes::Bass)
                    {
                        // 41 to 59
//                        scaledPitch *= (59 - 41);
//                        scaledPitch += 41;
                        pitchMapping.set (1, 59);
                        pitchMapping.set (2, 55);
                        pitchMapping.set (3, 52);
                        pitchMapping.set (4, 48);
                        pitchMapping.set (5, 45);
                        pitchMapping.set (6, 41);
                    }
                    else if (PlaybackSettings::get().getCurrentClef() == MusiSyncEng::ClefTypes::Treble)
                    {
                        pitchMapping.set (1, 81);
                        pitchMapping.set (2, 77);
                        pitchMapping.set (3, 72);
                        pitchMapping.set (4, 69);
                        pitchMapping.set (5, 65);
                        pitchMapping.set (6, 62);
                    }

//                    Array<int> cMajor = {41,45,48,52,55,59,62,65,69,72,77,81}; //< really these are white spaces not cmajr
////                    Array<int> cMajor = {40,41,43,45,47,48,50,52,53,55,57,59,
////                                        60,62,64,65,67,69,71,72,76,77,79,81};
//                    while(!cMajor.contains((int)scaledPitch)){
//                        scaledPitch += 1;
//
//                        if(scaledPitch > 81) scaledPitch = 81;
//                    }

                    notePitches.add (PlaybackSettings::get().getTuningOffsetCMajorMIDIValue (pitchMapping[scaledPitch]));
                }
                
                for (int i = 0; i < noteDurationOut.size(); ++i)
                {
                    noteDurations.add((float)noteDurationOut[i] / 248.0);
                }
                //========================================================================
                // Make Audio Happen
                
                float timeStamp = PlaybackSettings::get().getLatestTimestampAddition(); // Timestamp must be offset by a previous value
                float noteLengthAccumulator = 0.0;
                for (int i = 0; i < notePitches.size(); ++i)
                {
                    // translate into message on
                    auto messageOn = MidiMessage::noteOn ((int)PlaybackSettings::get().getInstrument(),
                                                          notePitches[i],
                                                          (uint8) PlaybackSettings::get().getVelocity());
                    messageOn.setTimeStamp (timeStamp); //< add timestamp
                    MidiEventList::get().addMidiEvent (messageOn); //< add to event list
                    LiveGuiPlayback::get().add (nullptr, true, timeStamp); //< update gui feeback
                    LiveGuiPlayback::get().add (bar, true, timeStamp, visibleColumnIndicesOut[i]);
                    
                    // calculate the next value
                    timeStamp += PlaybackSettings::get().lengthOfNoteInMillisec (noteDurations[i]);
                    noteLengthAccumulator += noteDurations[i];
                    
                    // add note off
                    auto messageOff = MidiMessage::noteOff ((int)PlaybackSettings::get().getInstrument(),
                                                            notePitches[i],
                                                            (uint8) PlaybackSettings::get().getVelocity());
                    messageOff.setTimeStamp (timeStamp); //< add timestamp
                    MidiEventList::get().addMidiEvent (messageOff); // add to event list
                    LiveGuiPlayback::get().add (nullptr, false, timeStamp); //< update gui feeback...
                    LiveGuiPlayback::get().add (bar, false, timeStamp, visibleColumnIndicesOut[i]);
                    
                    // update latest addition
                    PlaybackSettings::get().setLatestTimestampAddition (timeStamp);
                }
                //========================================================================
                
                // add space at the end of the bar
                // timestamp must be offest by previous value
                timeStamp = PlaybackSettings::get().getLatestTimestampAddition();

                // create a dummy message
                auto message = MidiMessage::noteOn ((int)PlaybackSettings::get().getInstrument(),
                                                    0, (uint8) 0);
                message.setTimeStamp (timeStamp);
                MidiEventList::get().addMidiEvent (message);
                LiveGuiPlayback::get().add (nullptr, true, timeStamp); //< update gui feeback
                LiveGuiPlayback::get().add (bar, true, timeStamp, -1);


                // calculate the next value
                timeStamp += PlaybackSettings::get().lengthOfNoteInMillisec (1.0 - noteLengthAccumulator);

                auto messageOff = MidiMessage::noteOff ((int)PlaybackSettings::get().getInstrument(),
                                                        0, (uint8) 0);
                messageOff.setTimeStamp (timeStamp);
                MidiEventList::get().addMidiEvent (messageOff);
                LiveGuiPlayback::get().add (nullptr, false, timeStamp); //< update gui feeback
                LiveGuiPlayback::get().add (bar, false, timeStamp, -1);
                
                // update latest addition
                PlaybackSettings::get().setLatestTimestampAddition (timeStamp);
            }
                    
            /**
             *  Contains info for saving and loading a four four bar
             *  @param the head element for this block
             *  @param if save or load should be performed
             */
            void doSaveOrLoad (XmlElement* blockHead, juckly::Block::FileManipulator mode) override
            {
                
                
                if (mode == juckly::Block::FileManipulator::save)
                {
                    auto bar = dynamic_cast<BlockComponent*>(getInternalUI());
                    NoteButtonColumn* columns = bar->getNoteButtonColumn();
                    for (int col = 0; col < bar->m_nCols; ++col)
                    {
                        XmlElement* rect = new XmlElement ("Rect");
                        rect->setAttribute ("colNo", col);
                        rect->setAttribute ("ActiveIndex", columns[col].getActiveButtonIndex());
                        rect->setAttribute ("width", columns[col].getActiveButtonWidth());
                        blockHead->addChildElement (rect);
                    }
                }
                
                
                if (mode == juckly::Block::FileManipulator::load)
                {
                    forEachXmlChildElement (*blockHead, e)
                    {
                        auto bar = dynamic_cast<BlockComponent*>(getInternalUI());
                        NoteButtonColumn* columns = bar->getNoteButtonColumn();
                        
                        int col =  e->getIntAttribute("colNo");
                        int activeIndex = e->getIntAttribute("ActiveIndex");
                        int width = e->getIntAttribute("width");
                        
                        if (activeIndex != -1){
                            columns[col].noteButtonClicked (activeIndex);
                            columns[col].setBounds((248 / 8) * col, 0, width, bar->getHeight());// (width);
                            bar->onResizeEnd(col);
                        }

                    }
                }
            }
        };
    
    //========================================================================================================
    //========================================================================================================
    //========================================================================================================
    
    class PainterlyBar    : public juckly::Block
    {
    public:
        
        /** Default constructor. */
        PainterlyBar () : juckly::Block (std::make_unique<juckly::CommandBlockSettings>
                                            ("PainterlyBar",
                                               ImageCache::getFromMemory(BinaryData::paintBlockIcon_png, BinaryData::paintBlockIcon_pngSize),
                                               ImageCache::getFromMemory (BinaryData::fourFourBlock_png,  BinaryData::fourFourBlock_pngSize),
                                               false /* not a start node */,
                                               new DrawComponent(),
                                               false /* dosen't take a parameter */,
                                               352
                                            )
                                        )
        {
            // scale connections to fit the image used
            getScaling().setConnectionWidthScaling (0.045);
        }
        
        /** Calculates the timing for a bar. */
        void doAction() override
        {
            //========================================================================
            // Get the things from Amy's bar...
            float noteDurationOut = 0;
            juce::Array<int> notePitchOut;
            juce::Array<juce::Rectangle<int>> noteRectangleOut;
            
            auto block = dynamic_cast<DrawComponent*>(getInternalUI());
            block->getAudioInfo (noteDurationOut, notePitchOut, noteRectangleOut);
            block->repaint();
            
            //========================================================================
            
            //========================================================================
            // Scale to values for Codetta...
            float noteLength = noteDurationOut / 248;
            
            juce::Array<int> notePitches;
            for (int i = 0; i < notePitchOut.size(); ++i)
            {
                if (notePitchOut[i] == -1)
                {
                    notePitches.add (0);
                }
                else
                {
                    // Scale to between 0 and 1
                    float scaledPitch = 1.0 - ((notePitchOut[i] - 27.0) / 47.0);
                    
                    if (PlaybackSettings::get().getCurrentClef() == MusiSyncEng::ClefTypes::Bass)
                    {
                        scaledPitch *= (60 - 40); scaledPitch += 40; // 40 to 60
//                        scaledPitch *= (59 - 41); scaledPitch += 41; // 41 to 59
                    }
                    else if (PlaybackSettings::get().getCurrentClef() == MusiSyncEng::ClefTypes::Treble)
                    {
                        scaledPitch *= (81 - 60); scaledPitch += 60; // 60 to 81
//                        scaledPitch *= (81 - 62); scaledPitch += 62; // 62 to 81
                    }
                 
                    Array<int> cMajor = {40,41,43,45,47,48,50,52,53,55,57,59, //TODO: painterly could be cmaj (see above lines too)
                                        60,62,64,65,67,69,71,72,76,77,79,81};
//                    Array<int> cMajor = {41,45,48,52,55,59,62,65,69,72,77,81};
                    while(!cMajor.contains((int)scaledPitch)){
                        scaledPitch += 1;
                        
                        if(scaledPitch > 81) scaledPitch = 81;
                    }
                        
                    notePitches.add (PlaybackSettings::get().getTuningOffsetCMajorMIDIValue ((int)scaledPitch));
                }
            }
            //========================================================================
            
            //========================================================================
            // Convert to midi playback...
            float timeStamp = PlaybackSettings::get().getLatestTimestampAddition(); // Timestamp must be offset by a previous value
            for (int i = 0; i < notePitches.size(); ++i)
            {
                // translate into message on
                auto messageOn = MidiMessage::noteOn ((int)PlaybackSettings::get().getInstrument(),
                                                      notePitches[i],
                                                      (uint8) PlaybackSettings::get().getVelocity());
                messageOn.setTimeStamp (timeStamp); //< add timestamp
                MidiEventList::get().addMidiEvent (messageOn); //< add to event list
                LiveGuiPlayback::get().add (nullptr, true, timeStamp); //< update gui feeback
                LiveGuiPlayback::get().add (block, true, timeStamp, i);
                
                // calculate the next value
                timeStamp += PlaybackSettings::get().lengthOfNoteInMillisec (noteLength);
                
                // add note off
                auto messageOff = MidiMessage::noteOff ((int)PlaybackSettings::get().getInstrument(),
                                                        notePitches[i],
                                                        (uint8) PlaybackSettings::get().getVelocity());
                messageOff.setTimeStamp (timeStamp); //< add timestamp
                MidiEventList::get().addMidiEvent (messageOff); // add to event list
                LiveGuiPlayback::get().add (nullptr, false, timeStamp); //< update gui feeback...
                LiveGuiPlayback::get().add (block, false, timeStamp, i);
                
                // update latest addition
                PlaybackSettings::get().setLatestTimestampAddition (timeStamp);
            }
            //========================================================================
        }
                
        /**
         *  Contains info for saving and loading a four four bar
         *  @param the head element for this block
         *  @param if save or load should be performed
         */
        void doSaveOrLoad (XmlElement* blockHead, juckly::Block::FileManipulator mode) override
        {
            auto bar = dynamic_cast<DrawComponent*>(getInternalUI());
            
            if (mode == juckly::Block::FileManipulator::save)
            {
                auto paint = bar->getInkCoords();
                for (auto& p : paint)
                {
                    XmlElement* paint = new XmlElement ("Paint");
                    paint->setAttribute ("x", p.x);
                    paint->setAttribute ("y", p.y);
                    blockHead->addChildElement (paint);
                }
                
                XmlElement* lines = new XmlElement ("NumLines");
                lines->setAttribute ("value", bar->getNoLines());
                blockHead->addChildElement (lines);
            }
            
            //==================================================================
            
            if (mode == juckly::Block::FileManipulator::load)
            {
                juce::Array<juce::Point<int>> newCoords;
                 forEachXmlChildElement (*blockHead, e)
                 {
                     if(e->hasTagName("Paint"))
                     {
                         int x = e->getIntAttribute("x");
                         int y = e->getIntAttribute("y");
                         
                         Point<int> newPoint;
                         newPoint.x = x;
                         newPoint.y = y;
                         newCoords.add(newPoint);
                     }
                     
                     if(e->hasTagName("NumLines"))
                     {
                         int numLines = e->getIntAttribute("value");
                         bar->setNoLines(numLines);
                     }
                 }
                bar->setInkCoords (newCoords);
            }
        }
    };
    } // namespace amy

} // namespace codetta
