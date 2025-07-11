/*
  ==============================================================================

    DataLogger.h
    Created: 12 Nov 2019 5:01:53pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "DataLoggerViewer.h"

//========================================================================
#undef USE_LOGGING

// Pre-processors for my Masters study
#undef LOCK_LOGGING_TO_DOCUMENTS // Prepped for MRes study - on for pilot
#undef WESSEX_LOG // Prepped for MRes study - on for Main Study

// Pre-processors for my PhD studies
#undef PHD_STUDY_1 // Prepped for the first study of my PhD
#undef AMY_VERSION_1
//========================================================================


/**
 *  Generic data logging class for mouse events.
 */
class DataLogger
{
public:
    /** Constructor */
    DataLogger();
    /** Destructor. Writes data to file. */
    ~DataLogger();
    
    // Pre-processor macro definitions! USE THESE INSTEAD OF
    // THE FUNCTIONS SO YOU CAN EASILY UNCOMPILE THEM LATER ON !!!!
    #ifdef USE_LOGGING
    #define LOG_STRING(A) DataLogger::getLogger().logString(A)
    #define RESET_LOG() DataLogger::getLogger().resetLog()
    #define WRITE_CSV(A) DataLogger::getLogger().writeCSV(A)
    #else
    #define LOG_STRING(A)
    #define RESET_LOG()
//    #define UPDATE_FILE_PATH(A)
    #define WRITE_CSV(A)
    #endif
        
    // Dont use any of these functions directly, use the macros!
    /**
     *  Singleton Getter
     *  @return this logger object!
     */
    static DataLogger& getLogger()
    {
        static DataLogger logger;
        return logger;
    }
    
    /**
     *  Writes the string description to the final output string, with a timestamp and X and Y mouse
     *  co-ordinates.
     *  @param The description to be added to the log.
     */
    void logString (String description);

    /** Sets the final output to an empty string. */
    void resetLog();
    
    /** Writes log to a csv file. */
    void writeCSV(String filePath);

private:
    /** Final string of interactions to be written to a file. */
    String finalOutput;
    /** The time in which the program was launched (data logger constructed).*/
    double launchTime;
};

