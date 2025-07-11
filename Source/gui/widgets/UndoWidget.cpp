/*
  ==============================================================================

    UndoWidget.cpp
    Created: 16 Mar 2021 4:22:51pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "UndoWidget.h"

//==============================================================================
/** Main namespace for codetta. */
namespace codetta
{
    UndoWidget::UndoWidget()
    {
        listener = nullptr;
        Image normalButton = ImageCache::getFromMemory (BinaryData::undoButtonNormal_png, BinaryData::undoButtonNormal_pngSize);
        Image overButton = ImageCache::getFromMemory (BinaryData::undoButtonOver_png, BinaryData::undoButtonOver_pngSize);
        Image downButton = ImageCache::getFromMemory (BinaryData::undoButtonDown_png, BinaryData::undoButtonDown_pngSize);
        setImages (false, true, true, normalButton, 1.0f, {}, overButton, 1.0f, {}, downButton, 1.0f, {});
    }

    UndoWidget::~UndoWidget()
    {
        listener = nullptr;
    }

    void UndoWidget::addListener(Listener* listenerRhs)
    {
        listener = listenerRhs;
    }

    void UndoWidget::updateUndoStack()
    {
        if (currentlyUndoing == false)
        {
            XmlElement fake ("fake");
            auto snapshot = listener->onUndoCommand (true, fake);
            
            snapshots.push_front (snapshot);
            
            if (snapshots.size() > bufferSize)
            {
                snapshots.pop_back();
            }
        }
    }

    void UndoWidget::rollback()
    {
        if (snapshots.size() > 1)
        {
            snapshots.pop_front();
            
            listener->onUndoCommand (false, snapshots.front());
        }
    }

    void UndoWidget::clear()
    {
        snapshots.clear();
    }

    bool UndoWidget::isUndoing()
    {
        return currentlyUndoing;
    }

    void UndoWidget::setIsUndoing(bool state)
    {
        currentlyUndoing = state;
    }

}  //namespace codetta
