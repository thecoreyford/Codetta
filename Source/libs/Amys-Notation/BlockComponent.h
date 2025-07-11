/*
  ==============================================================================

    BlockComponent.h
    Created: 26 May 2021 5:42:42pm
    Author:  amysh

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../data-logger/DataLogger.h" //< added by corey
#include "../../gui/widgets/UndoWidget.h" //< added by corey

////// SUMMARY //////
// BlockComponent has 8 NoteButtons
// Each NoteButtonColumn has 6 NoteButtons (implemented as radio buttons so only 0-1 are active)
// NoteButtonColumn can be resized on the right edge using a ResizableEdgeComponent
// The ResizableEdge is constrained by the max/min width of the ResizableEdgeConstrainer
// The callback from ResizableEdgeConstrainer to BlockComponent is to implement the 
    // column hiding behaviour (NoteButtonColumn visibility is updated in BlockComponent)
    // when a resizeEnd() event happens (a built in member function of the ResizableEdgeConstrainer)
// The audio should identify the column width => this gives the duration
    // and the activeButtonIndex => this gives the pitch which is an int between 0 and 5 
    // if activeButtonIndex = -1 then play no sound as no button is selected
// The functions getAudioInfo(), getInteractionStats() were implemented to provide the values needed to integrate with Codetta
// The function highlightButtonOnPlay() should be called when playing the note

class IResizeEndCallback // Interface class for resizing callback
{
public:
    virtual void onResizeEnd(int resizedColIndex) = 0; // this function is defined by Block Component
};


class ResizableEdgeConstrainer : public juce::ComponentBoundsConstrainer
{
public:
    ResizableEdgeConstrainer()
        : m_resizeCallback(nullptr)
        , m_colIndex(-1)
    {
    }
    
    ~ResizableEdgeConstrainer()
    {
    }

    void setResizeEndCallback(IResizeEndCallback* callback, int colIndex)
    {
        m_colIndex = colIndex;
        m_resizeCallback = callback;
    }

    virtual void resizeEnd() override
    {
        if (m_resizeCallback == nullptr || m_colIndex < 0) // if no value is set, then exit the function
            return;
        m_resizeCallback->onResizeEnd(m_colIndex);
    }
private:
    IResizeEndCallback* m_resizeCallback;
    int m_colIndex;
};


class ResizableEdge : public juce::ResizableEdgeComponent
{
public:
    ResizableEdge(juce::Component* parent)
        : juce::ResizableEdgeComponent(parent, &m_resizableEdgeConstrainer, juce::ResizableEdgeComponent::rightEdge)
    {
        int blockWidth = 248;
        int minColumnWidth = blockWidth / 8;
        m_resizableEdgeConstrainer.setMinimumWidth(minColumnWidth);
        m_resizableEdgeConstrainer.setMaximumWidth(blockWidth);
        m_resizableEdgeConstrainer.setMinimumOnscreenAmounts(0, 0, 0, blockWidth + 1); // this value >248 prevents it going off the right side of screen
    }

    ~ResizableEdge()
    {
    }

    void setResizeEndCallback(IResizeEndCallback* callback, int colIndex) // used for callback ResizableEdge -> ResizableEdgeConstrainer
    {
        m_resizableEdgeConstrainer.setResizeEndCallback(callback, colIndex);
    }

private:
    ResizableEdgeConstrainer m_resizableEdgeConstrainer;
};

class NoteButton : public juce::ShapeButton
{
public:
    NoteButton()
        : juce::ShapeButton("noteButton", juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f), juce::Colours::indianred, juce::Colours::darkred)
    {
    }

    ~NoteButton()
    {
    }
};

class NoteButtonColumn : public juce::Component//, juce::ComponentListener
{
public:
    NoteButtonColumn()
        : m_columnRadioGroupID(1)
        , m_noteColResizableEdge(this) // this noteButtonColumn is passed in as the component to resize
        , m_activeButtonIndex(-1)
    {
        setSize(248 / 8, 80); // (parent width 248 / n columns 8 , parent height 80)

        // add NoteButtons
        const float buttonHeight = getHeight() / m_nRows;
        const float buttonWidth = getWidth();
        for (int row = 0; row < m_nRows; row++)
        {
            createNoteButton(row, buttonWidth, buttonHeight);
            m_noteButton[row].setBorderSize(juce::BorderSize<int>(buttonHeight / 2, 4, 0, 0));
            m_noteButton[row].setTopLeftPosition(0, row * buttonHeight); 
            m_noteButton[row].setRadioGroupId(m_columnRadioGroupID); // now each button in the column is mutually exclusice
            m_noteButton[row].onClick = [this, row] { noteButtonClicked(row); };
        };

        // make resizable (size/pos added later)
        addAndMakeVisible(m_noteColResizableEdge);
    }

    ~NoteButtonColumn()
    {
    }

    void createNoteButton(int i, float width, float height)
    {
        addAndMakeVisible(m_noteButton[i]);
        // set shape to be a rectangle
        juce::Path shapePath; // A path is a sequence of lines and curves
        shapePath.addRectangle(0.f, 0.f, width, height);
        m_noteButton[i].setShape(shapePath, false, false, false);
        m_noteButton[i].setSize(width, height);
        // set toggle on colours
        m_noteButton[i].setClickingTogglesState(true); // automatically flip the toggle state when the button is clicked
        m_noteButton[i].setOnColours(juce::Colours::darkred, juce::Colours::darkred, juce::Colours::lightgrey);
        m_noteButton[i].shouldUseOnColours(true);
    }

    void updateResizableEdgeBounds() // called by noteButtonClicked()
    {
        if (m_activeButtonIndex >= 0) // if no buttons have been selected then there is no resizable edge
        {
            // update position & size of resizable edge
            m_noteColResizableEdge.setSize(m_resizableEdgeWidth, getHeight() / m_nRows * 1.4); // add 40% extra height
            updateResizableEdgePos();
        }
    }

    void updateResizableEdgePos() // called by updateResizableEdgeBounds() when note button is clicked and by resized()
    {
        m_noteColResizableEdge.setTopLeftPosition(getWidth() - m_resizableEdgeWidth, m_noteButton[m_activeButtonIndex].getY());
    }

    void noteButtonClicked(int i) // called when button i is clicked
    {
        //corey ---- for saving and loading
        m_noteButton[i].setToggleState(false, dontSendNotification);
        if (i != -1){m_noteButton[i].setToggleState(true, dontSendNotification); }
        //------------------------------
        
        m_activeButtonIndex = i; // in a radio group, the button that was clicked is now the active button
        updateResizableEdgeBounds(); // update position of resizable edge to match new Y coord
        m_pitchEditCount += 1; // used for stats only
        LOG_STRING("Edited piano roll pitch"); //< cf
        codetta::UndoWidget::get().updateUndoStack(); //< cf

    }

    void setResizeEndCallback(IResizeEndCallback* callback, int colIndex) // used for callback noteButtonColumn -> ResizableEdge
    {
        m_noteColResizableEdge.setResizeEndCallback(callback, colIndex);
    }

    void resized() override // children (note buttons and resizable edge) need to be responsive to resizing
    {
        // update width of each note button
        for (int row = 0; row < m_nRows; row++)
        {
            m_noteButton[row].setSize(getWidth(), getHeight() / m_nRows);
        };
        updateResizableEdgePos(); // update position of resizable edge to match new X coord
        m_rhythmEditCount += 1;
        LOG_STRING("Edited piano roll rhythm"); //<cf
        codetta::UndoWidget::get().updateUndoStack(); //< cf
        
    }

    ///// to be used for integration with codetta /////
    void getInteractionStats(int& rhythmEditCountOut, int& pitchEditCountOut)
    {
        // number of resize drags and number of button clicks for stats
        rhythmEditCountOut = m_rhythmEditCount;
        pitchEditCountOut = m_pitchEditCount;
    }

    //===================
    bool isNoteOn;
    void highlightButtonOnPlay (int activeIndex, bool noteOn)  // should be called when note is played //COREY MODDED
    {
        isNoteOn = noteOn;
        repaint();
    }
    
    int getActiveButtonY() { return m_noteButton[m_activeButtonIndex].getY(); }
    int getActiveButtonX() { return m_noteButton[m_activeButtonIndex].getX(); }
    int getActiveButtonHeight() { return m_noteButton[m_activeButtonIndex].getHeight(); }
    int getActiveButtonWidth() { return m_noteButton[m_activeButtonIndex].getWidth(); }
    //===================
    
    int getActiveButtonIndex() // called by getAudioInfo only
    {
        return m_activeButtonIndex;
    }

private:
    static const int m_nRows = 6; // static so we can use in the next line
    NoteButton m_noteButton[m_nRows];
    ResizableEdge m_noteColResizableEdge;
    const int m_columnRadioGroupID;
    const int m_resizableEdgeWidth = 15;
    int m_activeButtonIndex;
    // for stats
    int m_rhythmEditCount;
    int m_pitchEditCount;
};

class BlockComponent : public juce::Component, public IResizeEndCallback
{
public:
    BlockComponent()
    {
        setSize(248, 80); // set block size (FourFourBar)

        // add NoteButtonColumns
        const float buttonWidth = getWidth() / m_nCols;
        for (int i = 0; i < m_nCols; ++i)
        {
            addAndMakeVisible(m_noteButtonColumn[i]);
            m_noteButtonColumn[i].setTopLeftPosition(buttonWidth * i, 0);
            // an instance of this IResizeEndCallback is assigned to m_resizeCallback (private member of the constrainer)
            m_noteButtonColumn[i].setResizeEndCallback(this, i);

        }
    }

    ~BlockComponent() override
    {
    }

    virtual void onResizeEnd(int resizedColIndex) // called when resizeEnd is called on the constrainer
    {
        // hides columns that are covered by a previous column that has been expanded
        // start with the column that has been resized (resizedColIndex)
        int rightEdgePrev_i = m_noteButtonColumn[resizedColIndex].getX() + m_noteButtonColumn[resizedColIndex].getWidth();
        for (int i = resizedColIndex + 1; i < m_nCols; ++i)
        {
            // if previous right edge > current left edge, hide column i
            // the right edge to be compared is not updated
            int leftEdge_i = m_noteButtonColumn[i].getX();
            if (rightEdgePrev_i > leftEdge_i)
            {
                m_noteButtonColumn[i].setVisible(false);
            }
            // else update the right edge to be compared
            else
            {
                m_noteButtonColumn[i].setVisible(true); // reverses in case it was false
                rightEdgePrev_i = m_noteButtonColumn[i].getX() + m_noteButtonColumn[i].getWidth();
            }
        }
    }

    void drawStaveLines(juce::Graphics& g) // called by paint
    {
        // reference (Ford, 2018)
        g.setColour(juce::Colours::black);

        const float thickness = getHeight() / 12.0f;

        for (int i = 1; i <= 10; ++i)
        {
            if (i % 2 == 0)
                g.drawLine(0, i * thickness + (thickness * 0.25f), (float)getWidth(),
                    i * thickness + (thickness * 0.25f), thickness * 0.5f);
        };
        
        //Corey's Code....
        if (isNoteOn)
        {
            g.setColour(juce::Colours::lightgreen);
            g.fillRect(playbackX,
                       playbackY,
                       playbackWidth + 4,
                       18);
        }
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::white); // set background colour to white
        drawStaveLines(g);
    }

    void resized() override // children (note columns) do not need to be responsive to resizing of the block
    {
    }

    ///// to be used for integration with codetta /////
    void getAudioInfo(juce::Array<int>& noteDurationOut, juce::Array<int>& notePitchOut, juce::Array<int>& visibleColumnIndicesOut)
    {
        juce::Array<int> noteDurationArray;
        juce::Array<int> notePitchArray;
        juce::Array<int> visibleColumnIndices;
        // for each noteColumn in the block component
        // if the column is visible (this changes whenever a column is resized)
        for (int i = 0; i < m_nCols; ++i)
        {
            if (m_noteButtonColumn[i].isVisible() && (m_noteButtonColumn[i].getActiveButtonIndex() != -1)) ///< CF Mod here
            {
                visibleColumnIndices.add(i);
                // identify the column width (round up to nearest 8th) => this gives the duration
                float noteDuration = m_noteButtonColumn[i].getWidth();
                int noteDurationRounded = roundUpToNthNote(noteDuration, m_nCols);
                noteDurationArray.add(noteDurationRounded);
                // identify the active noteButton in the noteColumn => this gives the pitch
                // (this changes whenever a noteButton is clicked)
                int pitch = m_noteButtonColumn[i].getActiveButtonIndex();
                notePitchArray.add(pitch);
            };
        };
        noteDurationOut = noteDurationArray;
        notePitchOut = notePitchArray;
        visibleColumnIndicesOut = visibleColumnIndices;
    }

    int roundUpToNthNote(float duration, int n) // to round up the the next eighth note (width is a multiple of 31)
    {
        int nthNote = getWidth() / n; // will be 31 for 8 notes
        int durationRounded = 0;
        for (int i = 0; i < n; ++i)
        {
            if (duration > nthNote * i && duration <= nthNote * (i + 1))  // e.g. if 31 < duration <= 62, then set durationRounded = 62
            {
                durationRounded = nthNote * (i + 1);
            };
        };
        return durationRounded;
    }
    
    //====================Corey================
    int playbackY;
    int playbackX;
    int playbackWidth;
    int isNoteOn = false;
    void highlightButtonOnPlay (int idx, bool noteOn)
    {
        isNoteOn = noteOn;
        playbackX = m_noteButtonColumn[idx].getX();
        playbackY = m_noteButtonColumn[idx].getActiveButtonY();
        playbackWidth = m_noteButtonColumn[idx].getActiveButtonWidth();
        repaint();
    }
    
    NoteButtonColumn* getNoteButtonColumn() {return m_noteButtonColumn;}
    
    
    static const int m_nCols = 8;
    
private:
    NoteButtonColumn m_noteButtonColumn[m_nCols] ;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlockComponent)
};
