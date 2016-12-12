#ifndef EVENTDATA_H
#define EVENTDATA_H

/*
 * This struct represents one entry in the history of execution
 * and contains all the information pertaining to one valid event
 */
struct EventData
{
    EventData(double y) :
        ly(y)
    {}

    double ly;

    bool operator <(const EventData &other) const
    {
        return ly < other.ly;
    }
};

/*
 * This container holds all the history of the algorithm
 * needed for visualization
 */
typedef std::vector <EventData> EventsData;

#endif // EVENTDATA_H
