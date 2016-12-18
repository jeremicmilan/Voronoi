#ifndef VEVENT_H
#define VEVENT_H

#include <iostream>
#include "vpoint.h"
#include "vparabola.h"

/*
 *   The class for storing place / circle event in event queue.
 *
 *   point        : the point at which current event occurs (top circle point
 *                  for circle event, focus point for place event)
 *   pe           : whether it is a place event or not
 *   y            : y coordinate of "point", events are sorted by this "y"
 *   arch         : if "pe", it is an arch above which the event occurs
 *   circleCenter : if not "pe", this represents the intersection in the diagram
 */

class VEvent
{
public:

    VPoint *point;
    bool pe;
    double y;
    VParabola *arch;
    VPoint *circleCenter;

    /*
     *   Constructor for the class
     *
     *   p   : point, at which the event occurs
     *   pev : whether it is a place event or not
     */

    VEvent(VPoint *p, bool pev)
    {
        point = p;
        pe = pev;
        y = p->y;
        arch = nullptr;
        circleCenter = nullptr;
    }

    ~VEvent()
    {
        delete circleCenter;
    }

    /*
     *   function for comparing two events (by "y" property)
     */

    struct CompareEvent : public std::binary_function <VEvent *, VEvent *, bool>
    {
        bool operator ()(const VEvent *l, const VEvent *r) const
        {
            return l->y < r->y;
        }
    };
};

#endif // VEVENT_H
