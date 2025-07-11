///*
//  ==============================================================================
//
//    EndRepeat.h
//    Created: 9 Oct 2019 12:26:51pm
//    Author:  Corey Ford
//
//  ==============================================================================
//*/
//
//#pragma once
//
//#include "../JuceLibraryCode/JuceHeader.h"
//#include "../audio/Audio.h"
//#include "../libs/juckly/client/Block.h"
//#include "../data-logger/DataLogger.h"
//#include "BlockSlider.h"
//
//#define CODETTA_MAX_REPEATS 7
//
///** Alias for note fragments. */
//using FragmentPtr = std::unique_ptr<MusiSyncEng::NoteFragment>;
//
////==============================================================================
////TODO: comment
///** Main namespace for codetta. */
//namespace codetta
//{
//    /**
//     *  View component for the repeat component.
//     *  Google MVC (the way Qt does it) for a rough idea of why this is.
//     */
//    class RepeatView : public Component,
//                       public Slider::Listener
//    {
//    public:
//        /**
//         *  Constructor.
//         *  @param the model component array
//         */
//        RepeatView (int* modelRhs)    : model(modelRhs)
//        {
//            setSize (175, 28 * CODETTA_MAX_REPEATS);
//            
//            #ifdef USE_LOGGING
//            for (int i = 0; i < CODETTA_MAX_REPEATS; i++){
//                slidersPrev[i] = -1;
//            }
//            #endif
//            
//            // You need to pass in a model
//            jassert (model != nullptr);
//            
//            for (int i = 0; i < CODETTA_MAX_REPEATS; ++i)
//            {
//                sliders.add (new Slider (Slider::SliderStyle::IncDecButtons,
//                                         Slider::TextEntryBoxPosition::TextBoxLeft));
//                sliders[i]->setColour (Slider::ColourIds::textBoxTextColourId,
//                                       Colours::black);
//                sliders[i]->setRange (-7.0, 14.0, 1.0);
//                sliders[i]->setValue (model[i]);
//                
//                addAndMakeVisible (sliders.getLast());
//                sliders[i]->setBounds (0, i * 28, 175, 28);
//                #ifdef USE_LOGGING
//                sliders[i]->addListener (this);
//                #endif
//            }
//        }
//        
//        /** Destructor */
//        ~RepeatView()
//        {
//            for (int i = 0; i < CODETTA_MAX_REPEATS; ++i)
//            {
//                model[i] = sliders[i]->getValue();
//            }
//        }
//        
//        //TODO: beautiful comments
//        void sliderValueChanged (Slider* slider) override
//        {
//            #ifdef USE_LOGGING
//            for (int i = 0; i < CODETTA_MAX_REPEATS; ++i)
//            {
//                if (slider == sliders[i])
//                {
//                    if (slidersPrev[i] > slider->getValue())
//                        LOG_STRING("slot " + (String)i + " decreased");
//                    else
//                        LOG_STRING("slot " + (String)i + " increased");
//                    
//                    //update prev's
//                    slidersPrev[i] = slider->getValue();
//                }
//            }
//            #endif
//        }
//        
//        /**
//         *  Paint function.
//         *  @param the graphics context.
//         */
//        void paint (Graphics& g) override
//        {
//            g.fillAll (Colours::lightgrey);
//        }
//        
//    private:
//        /** Pointer to the model for this view */
//        int* model;
//        
//        /** The slider components making up the view. */
//        OwnedArray<Slider> sliders;
//        
//        #ifdef USE_LOGGING
//        int slidersPrev[CODETTA_MAX_REPEATS];
//        #endif
//    };
//    
//    //==========================================================================
//    
//    /**
//     *  Model component for the repeat component.
//     *  Google MVC (the way Qt does it) for a rough idea of why this is.
//     */
//    class RepeatModel : public Component
//    {
//    public:
//        /** Constructor */
//        RepeatModel()
//        {
//            // ensure array is zero by default
//            for (int i = 0; i < CODETTA_MAX_REPEATS; ++i)
//                pitchShifts[i] = 0;
//            
//            // Setup slider
//            slider = std::make_unique<BlockSlider>("}", 1.0, CODETTA_MAX_REPEATS, 1.0, 1.0, -16, -3);
//            addAndMakeVisible (*slider);
//            
//            // Customise Button
//            showArray.setColour (TextButton::ColourIds::buttonColourId,
//                                 Colour::fromRGB (2,226,227));
//            showArray.setColour (TextButton::ColourIds::textColourOffId,
//                                 Colours::darkgrey);
//            showArray.setButtonText ("\\/");
//            addAndMakeVisible (showArray);
//
//            
//            // Launch array on click
//            showArray.onClick = [this]
//            {
//                LOG_STRING("array button clicked");
//                CallOutBox::launchAsynchronously (new RepeatView(pitchShifts), getScreenBounds(), nullptr);
//                //for (int i = 0; i < CODETTA_MAX_REPEATS; ++i)
//                  //  DBG (pitchShifts[i]);
//            };
//        }
//        
//        /** Resized function */
//        void resized() override
//        {
//            slider->setBounds (getLocalBounds());
//            showArray.setBounds (getLocalBounds()
//                                .withTrimmedRight (getWidth() * 0.5)
//                                .withTrimmedTop (getHeight() * 0.75));
//            
//        }
//        
//        /** Friend alowing the codetta block acesss to private data. */
//        friend class DemoNewRepeat;
//        
//    private:
//        /** Button triggering callout for view. */
//        TextButton showArray;
//        
//        /** Slider for the repeat values*/
//        std::unique_ptr<BlockSlider> slider;
//        
//        /** Data model for the array values. */
//        int pitchShifts[CODETTA_MAX_REPEATS];
//    };
//    
//    //==========================================================================
//    
//    /**
//     *  Class represening an end repeat bar.
//     */
//    class EndRepeat    : public juckly::Block
//    {
//    public:
//        /** Default constructor. */
//        EndRepeat () : juckly::Block ( std::make_unique<juckly::CommandBlockSettings>
//                                      ("EndRepeat",
//                                       ImageCache::getFromMemory (BinaryData::endRepeatBlockIcon_png,
//                                                                  BinaryData::endRepeatBlockIcon_pngSize),
//                                       ImageCache::getFromMemory (BinaryData::yellowBlock_png,
//                                                                  BinaryData::yellowBlock_pngSize),
//                                       false /* not a start node */,
//                                       new RepeatModel,
//                                       false /* dosen't take a parameter */
//                                       )
//                                      ){ }
//        
//        /** Logic for cycling from a start repeat block. */
//        void doAction() override
//        {
//            auto repeatModel = dynamic_cast<RepeatModel*>(getInternalUI());
//            auto slider = repeatModel->slider.get();
//            jassert (slider != nullptr); // Cast didn't work! :(
//            
//
//            if (slider != nullptr)
//            {
//                auto totalRepeats = slider->getValue();
//                
//                // for every specified repeat
//                for (int i = 0; i < (int)totalRepeats; ++i)
//                {
//                    // Set the tuning based no the repeat value
//                    PlaybackSettings::get().setTuningOffset (repeatModel->pitchShifts[i]);
//                    
//                    auto current = PlaybackSettings::get().getThisRepeat();
//                    
//                    if (current != nullptr)
//                    {
//                        //TODO: optimise as start block could simply be an instrument
//                        // cycle through up to this repeat
//                        while (current != this)
//                        {
//                            if (dynamic_cast<StartBlock*>(current)){
//                                dynamic_cast<StartBlock*>(current)->doResets();
//                            }
//                            else if (dynamic_cast<InstrumentBlock*>(current)){
//                                dynamic_cast<InstrumentBlock*>(current)->doResets();
//                            }
//                            else{
//                                current->doAction();
//                            }
//                            
//                            current = current->getConnection().getNextNode();
//                        }
//                        
//                    }
//                    // Ensure that we reset to zero too!!!
//                    PlaybackSettings::get().setTuningOffset (0);
//                }
//                
//                PlaybackSettings::get().deregisterStartRepeat();
//            }
//        }
//        
//        //======================================================================
//        
//        /**
//         *  Contains info for saving and loading the repeat block's internal data
//         *  @param the head element for this block
//         *  @param if save or load should be performed
//         */
//        void doSaveOrLoad (XmlElement* blockHead, FileManipulator mode) override
//        {
//            auto repeatModel = dynamic_cast<RepeatModel*>(getInternalUI());
//            auto slider = repeatModel->slider.get();
//            auto array = repeatModel->pitchShifts;
//            
//            if (mode == FileManipulator::save)
//            {
//                blockHead->setAttribute ("noRepeats", slider->getValue());
//                
//                XmlElement* pitchShiftsElm = new XmlElement("PitchShifts");
//                for (int i = 0; i < CODETTA_MAX_REPEATS; ++i){
//                    pitchShiftsElm->setAttribute("Shift-" + (String)i, array[i]);
//                }
//                
//                blockHead->addChildElement (pitchShiftsElm);
//            }
//            
//            //==================================================================
//            
//            if (mode == FileManipulator::load)
//            {
//                slider->setValue (blockHead->getIntAttribute("noRepeats"));
//               
//                forEachXmlChildElement (*blockHead, e) {
//                    if (e->hasTagName ("PitchShifts"))
//                    {
//                        for (int i = 0; i < CODETTA_MAX_REPEATS; ++i)
//                            array[i] = e->getIntAttribute("Shift-" + (String)i);
//                    }
//                }
//               
//            }
//        }
//        
//    private:
//    };
//    
//} // namespace codetta
//
