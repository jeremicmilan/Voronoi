#ifndef EVENTDATA_H
#define EVENTDATA_H

#include <vparabola.h>

/*
 * This struct represents one entry in the history of execution
 * and contains all the information pertaining to one valid event
 */
struct EventData
{
    EventData(
        double		y,
        VParabola * r = nullptr,
        bool		pe = true,
        VPoint *	cc = nullptr)
        :
        ly(y),
        root(r),
        isPointEvent(pe),
        circleCenter(VPoint(-1, -1))
    {
        if (nullptr != cc)
        {
            circleCenter = *cc;
        }
    }

    double		ly;
    VParabola * root; // We are owning this

    bool		isPointEvent;
    VPoint		circleCenter;

    bool operator <=(const EventData &other) const
    {
        return ly <= other.ly;
    }
};

/*
 * This container holds all the history of the algorithm
 * needed for visualization
 */
typedef std::vector <EventData> EventsData;

#endif // EVENTDATA_H
