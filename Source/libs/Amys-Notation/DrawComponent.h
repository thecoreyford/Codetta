/*
  ==============================================================================

    DrawComponent.h
    Created: 3 Jun 2021 3:04:57pm
    Author:  amysh
 
    Modifed by Corey Ford 17th June 2021

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../data-logger/DataLogger.h" //< added by corey
#include "../../gui/widgets/UndoWidget.h" //< added by corey

////// SUMMARY //////
// The DrawComponent has a green header section (height 22) for the rhythm and mode control buttons
// The rhythmButtons are implemented as radio buttons and update the number of vertical lines drawn when clicked
    // by calling updateNumVerticalLines(n) 
// The modeButtons are implemented as radio buttons and toggle m_eraseMode on/off when clicked by calling updateMode()
// When the mouse hovers over the drawing section, it displays a red or grey circle, depending on the mode
// When the mouse is dragged over the drawing section, it adds (in draw mode) or removes (in erase mode) Points of x,y coordinates 
    // from the m_inkCoordinates array
// m_inkCoordinates is painted as red circles
// The audio should identify the distance between the vertical lines => this gives the duration
    // and take the average y-value of the m_inkCoordinates between two vertical lines => this gives the pitch
        // The pitch will be an float between m_headerHeight (22) and block height (80)
        // if pitch = -1 then play no sound as identifyPitch() returns -1 if there is no ink
// The functions getAudioInfo() and getInteractionStats() were implemented to provide the values needed to integrate with Codetta
// The function drawPitchOnPlay() should be called when playing the note


class DrawComponent : public juce::Component
{
public:
    DrawComponent()
        : m_mouseHoverCoordinates(0,0)
        , m_eraseMode(false)
        , m_nLines(1)
        // for stats
        , m_noteDrawCount(0)
        , m_noteEraseCount(0)
        , m_rhythmEditCount(-1) // start at -1 as 1 click is triggered on initialization
    {
        setSize(248, 80); // set block size (FourFourBar)
        setTopLeftPosition(0, 120); // TODO: this is not applicable once integrated with Codetta

        // add button to toggle draw/erase
        addModeButton(m_drawModeButton);
        m_drawModeButton.setTopLeftPosition(getWidth() - m_modeButtonWidth * 2, m_headerPadding);
        m_drawModeButton.setButtonText("Draw");
        m_drawModeButton.triggerClick(); // initially act as if draw mode is clicked
        addModeButton(m_eraseModeButton);
        m_eraseModeButton.setTopLeftPosition(getWidth() - m_modeButtonWidth, m_headerPadding);
        m_eraseModeButton.setButtonText("Erase");
        
        // add 4 radio buttons to update number of vertical lines
        addRhythmButton(rhythmButton2, 2, 0 * m_rhythmButtonWidth + m_rhythmLabelWidth);
        rhythmButton8.triggerClick(); // initially act as if button8 is clicked
        addRhythmButton(rhythmButton4, 4, 1 * m_rhythmButtonWidth + m_rhythmLabelWidth);
        addRhythmButton(rhythmButton8, 8, 2 * m_rhythmButtonWidth + m_rhythmLabelWidth);
        addRhythmButton(rhythmButton16, 16, 3 * m_rhythmButtonWidth + m_rhythmLabelWidth);
      
        //======Added by Corey ========//
        isNoteOn = false;
        currentNoteIndex = 0;
    };

    ~DrawComponent()
    {
    };

    ///// draw 1-8 vertical lines /////   
    enum RadioButtonIds
    {
        rhythmButtonsRadioID = 1001, //A radio group ID is a non-zero integer that is used to identify the group of buttons than must be mutually exclusive.
        modeButtonsRadioID = 1002
    };

    void addRhythmButton(juce::TextButton& rhythmN, int n, int xPos)
    {
        addAndMakeVisible(rhythmN);
        rhythmN.setButtonText(juce::String(n));
        rhythmN.setSize(m_rhythmButtonWidth, m_headerHeight - m_headerPadding * 2);
        rhythmN.setTopLeftPosition(xPos, m_headerPadding);
        rhythmN.setClickingTogglesState(true);
        // (n-1) lines e.g. rhythm 4 has 3 lines which break the screen into 4 sections
        rhythmN.onClick = [=] {updateNumVerticalLines(n-1);};
        rhythmN.setRadioGroupId(rhythmButtonsRadioID);
        rhythmN.setColour(juce::TextButton::buttonColourId, juce::Colours::lightgrey);
        rhythmN.setColour(juce::TextButton::textColourOffId, juce::Colours::darkslategrey);
        rhythmN.setColour(juce::TextButton::buttonOnColourId, juce::Colours::darkslategrey);
        rhythmN.setColour(juce::TextButton::textColourOnId, juce::Colours::lightgrey);
    };

    void updateNumVerticalLines(int n) // called when a rhythmButton is clicked
    {
        codetta::UndoWidget::get().updateUndoStack(); //< cf
        m_nLines = n;
        repaint();
        m_rhythmEditCount += 1;
        LOG_STRING ("Rhythm changed to... " + (String)(n+1)); //< cf
    };
    
    void drawVerticalLines(juce::Graphics& g) // called by paint
    {
        g.setColour(juce::Colours::black);
        int verticalLineSpacing = getWidth() / (m_nLines + 1);
        for (int i = 0; i < (m_nLines); ++i)
        {
            g.fillRect((i + 1) * verticalLineSpacing, m_headerHeight, 2, getHeight()- m_headerHeight); // x,y,width,height
        }
    };

    ///// draw/erase mode button /////
    void addModeButton(juce::TextButton& modeButton)
    {
        addAndMakeVisible(modeButton);
        modeButton.setSize(m_modeButtonWidth, m_headerHeight - m_headerPadding * 2);
        modeButton.setClickingTogglesState(true);
        modeButton.onClick = [this] { updateMode(); };
        modeButton.setRadioGroupId(modeButtonsRadioID);
        modeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::lightgrey);
        modeButton.setColour(juce::TextButton::textColourOffId, juce::Colours::darkslategrey);
        modeButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::darkslategrey);
        modeButton.setColour(juce::TextButton::textColourOnId, juce::Colours::lightgrey);
    }

    void updateMode() // called when a modeButton is clicked
    {
        if (m_eraseModeButton.getToggleState())
        {
            m_eraseMode = true;
        }
        else
        {
            m_eraseMode = false;
        }
    }

    // to align the mouse pointer with the centre of the circle (not the default top left)
    // and ensure the circle stays within the drawing area
    void drawCircleAtMiddlePointer(juce::Graphics& g, juce::Point<int> mousePos)
    {
        if (mousePos.x > m_noteRadius && mousePos.y > m_noteRadius + m_headerHeight 
            && getWidth() - mousePos.x > m_noteRadius && getHeight() - mousePos.y > m_noteRadius)
        {
            const int noteDiameter = m_noteRadius * 2;
            g.fillEllipse(mousePos.x - m_noteRadius, mousePos.y - m_noteRadius, noteDiameter, noteDiameter); 
        }
    };

    ///// pink circle when hovering /////
    virtual void mouseMove(const juce::MouseEvent& hoverEvent) override// called when the mouse isn't pressed and moves over component
    {
        m_mouseDragCoordinates.setXY(0,0); // clear the current drag circle
        m_mouseHoverCoordinates.setXY(hoverEvent.x, hoverEvent.y);
        repaint();
    };

    void drawCursor(juce::Graphics& g, juce::Point<int> mouseHoverPos) // called by paint
    {
        if (m_eraseMode) 
        {
            g.setColour(juce::Colours::lightgrey);
        }
        else
        {
            g.setColour(juce::Colours::indianred);
        };
        drawCircleAtMiddlePointer(g, mouseHoverPos);
    };

    ///// red circle when dragging /////
    virtual void mouseDrag(const juce::MouseEvent& dragEvent) override  // called when mouse is dragged inside component
    {
        const int radiusSquared = m_noteRadius * m_noteRadius;
        m_mouseHoverCoordinates.setXY(0,0); // clear the hover
        m_mouseDragCoordinates.setXY(dragEvent.x, dragEvent.y);
        if (m_eraseMode == false) // if in draw mode then append x,y coordinates to the ink array
        {
            m_inkCoordinates.add(m_mouseDragCoordinates);
        }
        else // if in erase mode then remove from array all points within the circle radius
        {
            int i = 0;
            while (i < m_inkCoordinates.size())
            {
                const juce::Point<int>& note = m_inkCoordinates[i];
                if (note.getDistanceSquaredFrom(m_mouseDragCoordinates) < radiusSquared)
                {
                    m_inkCoordinates.remove(i);
                    continue;
                }
                ++i;
            }
        }
        repaint(); // redraw the whole component with the updated ink array
    };

    void drawCoordinates(juce::Graphics& g, juce::Array<juce::Point<int>> coordArray) // called by paint
    {
        for (int i = 0; i < coordArray.size(); ++i) // for every xy coordinate in ink, draw a red circle
        {
            g.setColour(juce::Colours::darkred);
            drawCircleAtMiddlePointer(g, coordArray[i]);
        }
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::white);
        // header section
        g.setColour(juce::Colours::green);
        g.fillRect(0, 0, getWidth(), m_headerHeight);
        g.setColour(juce::Colours::white);
        g.setFont(11);
        juce::String rhythmButtonLabel = "Rhythm:";
        g.drawText(rhythmButtonLabel, m_headerPadding, m_headerPadding, m_rhythmLabelWidth - m_headerPadding, m_headerHeight - m_headerPadding * 2, juce::Justification::right);
        juce::String modeButtonLabel = "Mode:";
        g.drawText(modeButtonLabel, getWidth() - m_modeButtonWidth * 3, m_headerPadding, m_modeButtonWidth, m_headerHeight - m_headerPadding * 2, juce::Justification::right);
        // drawing section
        drawVerticalLines(g);
        drawCursor(g, m_mouseHoverCoordinates);
        drawCursor(g, m_mouseDragCoordinates);
        drawCoordinates(g, m_inkCoordinates);

        //==================== Adpated here by Corey ....
        if (isNoteOn) {
            float a;
            juce::Array<int> b;
            juce::Array<juce::Rectangle<int>> c;
            
            g.setColour (juce::Colours::lightgreen);
            getAudioInfo (a, b, c);
            g.fillRect (c[currentNoteIndex]);
        }
            
    };

    void resized() override // children do not need to be responsive to resizing
    {
    };

    ///// to be used for integration with codetta /////
    void getAudioInfo(float& noteDurationOut, juce::Array<int>& notePitchOut, juce::Array<juce::Rectangle<int>>& noteRectangleOut)
    {
        // identify the pitch from m_inkCoordinates and the duration from m_nLines
        // all notes have the same duration so this is given as a float
            // but may have different pitches, so this is given as an array
        // when I erase, I remove the coordinates from m_inkCoordinates array, so dont need any extra steps here

        // width / n columns => this gives the duration
        float noteDuration = getWidth() / (m_nLines + 1);
        // for each column, take average y-axis => this gives the pitch
        juce::Array<int> notePitchArray;
        juce::Array<juce::Rectangle<int>> rectangleArray;
        for (int i = 0; i <= m_nLines; ++i)
        {
            float notePitch = identifyPitch(noteDuration * i, noteDuration);
            notePitchArray.add(notePitch);
            juce::Rectangle<int> rectangle = juce::Rectangle<int>(noteDuration * i, notePitch, noteDuration, 2 * m_noteRadius); // thickness of line is 2 * m_noteRadius = 10
            rectangleArray.add(rectangle);
        }
        noteDurationOut = noteDuration;
        notePitchOut = notePitchArray;
        noteRectangleOut = rectangleArray;        
    }

    void getInteractionStats(int& drawCountOut, int& eraseCountOut, int& editCountOut)
    {
        // count number of times mouse is down within the drawing zone
        drawCountOut = m_noteDrawCount;
        eraseCountOut = m_noteEraseCount;

        // count number of times rhythm is changed using the rhythm buttons 
        editCountOut = m_rhythmEditCount;
    }

    float identifyPitch(float xStart, float width) // called by getAudioInfo() only
    {
        // if the x coordinate falls within defined range, then sum the y coordinates and then average
        int sum = 0;
        int count = 0;
        for (juce::Point<int> point : m_inkCoordinates)
        {
            if (point.x > xStart && point.x <= xStart + width)
            {
                sum += point.y;
                count += 1;
            };
        };
        if (count == 0)  // if there are no ink points in this column, return zero
        {
            return -1.f;
        };
        float averageY = sum / count;
        return averageY; // will be between m_headerHeight (22) and block height (80)
    }

    void drawPitchOnPlay(juce::Graphics& g, juce::Rectangle<int> rectangle) // TODO: should be called when note is played
    {
        g.setColour(juce::Colours::lightgreen);
        g.fillRect(rectangle);
    }

    virtual void mouseDown(const juce::MouseEvent& mouseDownEvent) override // called when mouse is down // used for interaction stats only
    {
        codetta::UndoWidget::get().updateUndoStack();
        // if within drawing area, then update the stats variables
        if (mouseDownEvent.y > m_headerHeight)
        {
            if (m_eraseMode == false)
            {
                m_noteDrawCount += 1;
                LOG_STRING("Ink drawing clicked.");
            }
            else
            {
                m_noteEraseCount += 1;
                LOG_STRING("Ink erasing clicked.");
            }
        };
    }

    //CREATED BY COREY===============================
    float isNoteOn;
    int currentNoteIndex;
    void setRectDrawing (bool isNoteOnRhs, int index)
    {
        isNoteOn = isNoteOnRhs;
        currentNoteIndex = index;
        repaint();
    }
    
    juce::Array<juce::Point<int>> getInkCoords()
    {
        return m_inkCoordinates;
    }
    
    void setInkCoords(juce::Array<juce::Point<int>> newCoords)
    {
        m_inkCoordinates = newCoords;
    }
    
    
    int getNoLines()
    {
        return m_nLines;
    }
    
    void setNoLines(int noLines)
    {
        if (noLines == 1)  rhythmButton2.triggerClick();
        if (noLines == 3)  rhythmButton4.triggerClick();
        if (noLines == 7)  rhythmButton8.triggerClick();
        if (noLines == 15) rhythmButton16.triggerClick();
    }
    //===============================================
    
    
private:
    // for drawing
    juce::Array<juce::Point<int>> m_inkCoordinates;  // an array of x,y coordinates
    juce::Point<int> m_mouseDragCoordinates;
    juce::Point<int> m_mouseHoverCoordinates;
    const int m_noteRadius = 5;
    // for mode
    juce::TextButton m_drawModeButton;
    juce::TextButton m_eraseModeButton;
    bool m_eraseMode;
    // for rhythm
    juce::TextButton rhythmButton2;
    juce::TextButton rhythmButton4;
    juce::TextButton rhythmButton8;
    juce::TextButton rhythmButton16;
    int m_nLines;
    // for formatting
    const int m_headerHeight = 22;
    const int m_headerPadding = 2;
    const int m_rhythmLabelWidth = 42;
    const int m_rhythmButtonWidth = 22;
    const int m_modeButtonWidth = 34;
    // for stats
    int m_noteDrawCount;
    int m_noteEraseCount;
    int m_rhythmEditCount;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrawComponent);
};
