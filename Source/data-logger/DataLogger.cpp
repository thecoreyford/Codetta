/*
  ==============================================================================

    DataLogger.cpp
    Created: 12 Nov 2019 5:01:53pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "DataLogger.h"

DataLogger::DataLogger()
{
    finalOutput = "";
    launchTime = Time::getMillisecondCounterHiRes();
}


DataLogger::~DataLogger(){}

void DataLogger::writeCSV(String filePath)
{
	// TODO: link this to the file save stuff!
	File file;

	jassert (filePath != "none"); //you must have a file path!
	
	file = File(filePath.replace ("composition.xml", "interactions.csv"));

    FileOutputStream output (file);

    if (output.openedOk())
    {
        output.setNewLineString ("\n");

        // setup csv heading
        output.writeString ("timestamp,elapsedtime,x,y,description\n");
        
        output.writeString (finalOutput);

        output.flush(); // (called explicitly to force an fsync on posix)
    }
    
    #if defined JUCE_MAC && defined USE_LOGGING
    String myString = (String)"curl -X POST https://content.dropboxapi.com/2/files/upload \\\n"
                        + (String)"    --header \"Authorization: Bearer q4Cy6jD0XWgAAAAAAAAAAexsOAtOD50euF0z6ca7sg4EuQLJ8BA31By2i_hxdZCG"
    + (String)"\" \\\n" + "    --header \"Dropbox-API-Arg: {\\\"path\\\": \\\"/Phd-3/"
    + file.getFileName()
    + "\\\"}\" \\\n"
    + (String)"    --header \"Content-Type: application/octet-stream\" \\\n"
    + (String)"    -d \""
    + finalOutput
    + (String)"\"";
    
    system (myString.toRawUTF8());
    #endif
           
    
    // An error occoured in the FileOutputStream!
    jassert (output.getStatus().wasOk());

}


void DataLogger::logString (String description)
{
	String timestamp = Time::getCurrentTime().toString(true, true)
		+ ":"
		+ (String)Time::getCurrentTime().getMilliseconds();

    String elapsedTime = String::formatted("%f.2f", (Time::getMillisecondCounterHiRes() - launchTime));
    
	String mouseX = (String)Desktop::getMousePosition().getX();

	String mouseY = (String)Desktop::getMousePosition().getY();

	finalOutput += (String)(timestamp + ','
		                    + elapsedTime + ','
		                    + mouseX + ','
		                    + mouseY + ','
		                    + description.toLowerCase() + '\n');
	    
    // Make sure the interaction timestamp is displayed
    // in a way that can be easily noted at a glance.
    DataLoggerViewer::get().update (elapsedTime.upToFirstOccurrenceOf(".", false, false).dropLastCharacters(2) + "00");
}

void DataLogger::resetLog()
{
    finalOutput = "";
}


