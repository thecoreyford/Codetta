/*
  ==============================================================================

    Arcs.cpp
    Created: 19 Dec 2019 2:43:50pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "Arcs.h"

namespace codetta
{
    
    Arcs::~Arcs(){}
    
    void Arcs::paint (Graphics& g)
    {
        for (auto a : arcs)
        {
           Path myPath;
           g.setColour (Colours::yellow.withAlpha (a.transparency));
           myPath.startNewSubPath (a.startX, a.y);
           myPath.quadraticTo ( (a.endX + a.startX) / 2,
                                a.y - 70,
                                a.endX,
                                a.y );
           g.strokePath (myPath, PathStrokeType (5.0,
                                                 PathStrokeType::JointStyle::beveled,
                                                 PathStrokeType::EndCapStyle::rounded));
           if (a.y != 0 && a.endX != 0)
           {
               const int circleSize = 18;
               g.fillEllipse (a.endX - (circleSize * 0.5),
                              a.y - (circleSize * 0.5),
                              circleSize, circleSize);
           }
        }
    }
          
    void Arcs::resized(){}

    void Arcs::triggerArc (int y, int startX, int endX)
    {
        arcs.add ({y,startX,endX,1.0f});
        startTimer(100);
    }

    void Arcs::timerCallback()
    {
        for (int i = 0; i < arcs.size(); ++i)
        {
            arcs.setUnchecked(i, {arcs[i].y,
                                  arcs[i].startX,
                                  arcs[i].endX,
                                  arcs[i].transparency*0.85f});
            
            if (arcs[i].transparency < 0.0001)
                arcs.remove(i);
        }
        repaint();
    }

    void Arcs::stopDrawing()
    {
        stopTimer();
        arcs.clear();
        repaint();
    }

} // namespace codetta
