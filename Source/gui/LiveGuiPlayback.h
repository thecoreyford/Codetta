/*
  ==============================================================================

    LiveGuiPlayback.h
    Created: 3 Nov 2019 12:32:47pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once
#include "../libs/MusicSync-Notation-Engine/Note.h"
#include "../libs/Amys-Notation/DrawComponent.h"
#include "../libs/Amys-Notation/BlockComponent.h"
#include "Arcs.h"

//TODO: tidy up and comment this


namespace codetta
{

    struct NotePlayback
    {
        MusiSyncEng::Note* note;
        bool isNoteOn;
        float timestamp;
    };

    //==========================
    //Amy's blocks
    struct PainterlyPlayback
    {
        DrawComponent* component;
        bool isNoteOn;
        float timestamp;
        int index;
    };

    struct PianoRollPlayback
    {
        BlockComponent* component;
        bool isNoteOn;
        float timestamp;
        int index;
    };
    //==========================

    struct TimestampedArc
    {
        float timestamp;
        Arcs::ArcInfo info;
    };


//==============================================================================
//TODO: one day make this a template
/**
   *  Class used to sort the array in order of midi messages.
   *  @see  https://docs.juce.com/master/classArray.html
   */
  class NotePlaybackTimestampSorter
  {
  public:
      /**
       *  Returns a value determining what time stamp is larger.
       *  @param  lhs is the leftmost message to be compared.
       *  @param  rhs is the rightmost message to be compared.
       *  @return if the element is bigger, smaller or the same as/
       */
      static int compareElements(NotePlayback lhs, NotePlayback rhs)
      {
          if (lhs.timestamp < rhs.timestamp)
              return -1;
          else if (lhs.timestamp> rhs.timestamp)
              return 1;
          else // if a == b
              return 0;
      }
  };

/**
 *  Class used to sort the array in order of midi messages.
 *  @see  https://docs.juce.com/master/classArray.html
 */
class TimestampedArcSorter
{
public:
    /**
     *  Returns a value determining what time stamp is larger.
     *  @param  lhs is the leftmost message to be compared.
     *  @param  rhs is the rightmost message to be compared.
     *  @return if the element is bigger, smaller or the same as/
     */
    static int compareElements(TimestampedArc lhs, TimestampedArc rhs)
    {
        if (lhs.timestamp < rhs.timestamp)
            return -1;
        else if (lhs.timestamp> rhs.timestamp)
            return 1;
        else // if a == b
            return 0;
    }
};

/**
 *  Class used to sort the array in order of midi messages.
 *  @see  https://docs.juce.com/master/classArray.html
 */
class TimestampedPainterlySorter
{
public:
    /**
     *  Returns a value determining what time stamp is larger.
     *  @param  lhs is the leftmost message to be compared.
     *  @param  rhs is the rightmost message to be compared.
     *  @return if the element is bigger, smaller or the same as/
     */
    static int compareElements(PainterlyPlayback lhs, PainterlyPlayback rhs)
    {
        if (lhs.timestamp < rhs.timestamp)
            return -1;
        else if (lhs.timestamp> rhs.timestamp)
            return 1;
        else // if a == b
            return 0;
    }
};

/**
 *  Class used to sort the array in order of midi messages.
 *  @see  https://docs.juce.com/master/classArray.html
 */
class TimestampedPianoRollSorter
{
public:
    /**
     *  Returns a value determining what time stamp is larger.
     *  @param  lhs is the leftmost message to be compared.
     *  @param  rhs is the rightmost message to be compared.
     *  @return if the element is bigger, smaller or the same as/
     */
    static int compareElements(PianoRollPlayback lhs, PianoRollPlayback rhs)
    {
        if (lhs.timestamp < rhs.timestamp)
            return -1;
        else if (lhs.timestamp> rhs.timestamp)
            return 1;
        else // if a == b
            return 0;
    }
};

//==============================================================================

    //TODO: throw comments on this at some point!!!

    /**
     *  Container for gui feeback messages sorted by there timecode.
     */
    class LiveGuiPlayback
    {
       public:
        /**
         *  Getter for the singleton instance.
         *  @return reference to the instance.
         */
        static LiveGuiPlayback& get()
        {
            static LiveGuiPlayback instance;
            return instance;
        }
        
        void clearAllEvents()
        {
            list.clear();
            painterlyList.clear();
            pianoRollList.clear();
        }
            
        void add (MusiSyncEng::Note* note, bool isNoteOn, float timestamp)
        {
            NotePlayback notePlayback = {
                note,
                isNoteOn,
                timestamp
            };
            
            list.add (notePlayback);
            list.sort (sorter);
            //sortList();
        }
        
       void add (float timestamp, Arcs::ArcInfo arc)
       {
           TimestampedArc myArc = {timestamp, arc};
           arcList.add (myArc);
           arcList.sort (arcSorter);
       }
        
        void add (DrawComponent* component,
                  bool isNoteOn,
                  float timestamp,
                  int index)
        {
            PainterlyPlayback paint = {component, isNoteOn, timestamp, index};
            painterlyList.add (paint);
            painterlyList.sort (painterlySorter);
        }
        
        void add (BlockComponent* component,
                  bool isNoteOn,
                  float timestamp,
                  int index)
        {
            PianoRollPlayback bar = {component, isNoteOn, timestamp, index};
            pianoRollList.add (bar);
            pianoRollList.sort (pianoRollSorter);
        }
        
        PainterlyPlayback getPaintNote (const int index)
        {
            return painterlyList[index];
        }
        
        PianoRollPlayback getPianoRollBlock (const int index)
        {
            return pianoRollList[index];
        }
        
        MusiSyncEng::Note* getNote (const int index) const
        {
            // the index you are getitng is out of range!!!
            jassert (index < list.size());
            
            return list[index].note;
        }
        
        bool isNoteOn (const int index) const
        {
            // the index you are getitng is out of range!!!
            jassert (index < list.size());
           
            return list[index].isNoteOn;
        }
    
        void setAllNotesBlack()
        {
            for (auto n  : list){
                if (n.note != nullptr)
                    n.note->setCurrentColour (Colours::black);
            }
        }
        
        const Array<TimestampedArc>& getTimestampedArcList() const { return arcList; }
        
        void clearTimestampedArcList() {arcList.clear();}
        
    private:
      NotePlaybackTimestampSorter sorter;
        
      TimestampedArcSorter arcSorter;
        
      TimestampedPainterlySorter painterlySorter;
      
      TimestampedPianoRollSorter pianoRollSorter;
        
      void sortList()
      {
          bool sorted = false;
          while (sorted == false)
          {
              sorted = true;
              if ((list.size() - 1) != 0)
              {
                  for (int i = 0; i < list.size() - 1; ++i)
                  {
                      if (list[i].timestamp > list[i+1].timestamp)
                      {
                            //do swap
                            auto temp = list[i];
                            list[i] = list[i+1];
                            list[i+1] = temp;
                            sorted = false;
                       }
                  }
              }
          }
        }
        
        Array<NotePlayback> list;
        
        Array<TimestampedArc> arcList;
        
        Array<PainterlyPlayback> painterlyList;
        
        Array<PianoRollPlayback> pianoRollList;
        
    };

} // namespace codetta



