#include "model.h"

Model::Model(int w, int h, int n) :
    width(w),
    height(h),
    numOfPoints(n),
    animationParameter(0)
{
    voronoi = new Voronoi();
    vertices = new Vertices();
    scene = new QGraphicsScene(0, 0, w, h);

    srand((unsigned)time(NULL));
}

double Model::Width() const
{
    return width;
}

double Model::Height() const
{
    return height;
}

void Model::SetNumOfPoints(int n)
{
    numOfPoints = n;
}

void Model::SetAnimationParameter(double value)
{
    animationParameter = value;
}

QGraphicsScene *Model::Scene()
{
    return scene;
}

void Model::Clear()
{
    for (VPoint *point : *vertices)
    {
        delete point;
    }

    vertices->clear();

    eventsData.clear();
}

void Model::Init()
{
    for (int i = 0; i < numOfPoints; i++)
    {
        vertices->push_back(new VPoint(width * (double)rand() /
            (double)RAND_MAX,
            height * (double)rand() / (double)RAND_MAX));
    }

    edges = voronoi->GetEdges(vertices, (int)width, (int)height, eventsData);
    std::cout << "Voronoi diagram done!" << std::endl;

    for (EventData ed : eventsData)
    {
        std::cout << ed.ly << " ";
    }

    std::cout << std::endl;
}

void Model::DrawPoint(VPoint *point)
{
    double x = point->x;
    double y = height - point->y;

    scene->addRect(x - POINT_SIZE / 2,
        y - POINT_SIZE / 2,
        POINT_SIZE,
        POINT_SIZE);
}

void Model::DrawLine(const VEdge *edge)
{
    double startX = edge->start->x;
    double startY = height - edge->start->y;
    double endX = edge->end->x;
    double endY = height - edge->end->y;

    scene->addLine(startX, startY, endX, endY);
}

EventData &Model::FindEventData()
{
    // Add a check for last event
    if (true)
    {
        return *std::lower_bound(eventsData.begin(), eventsData.end(),
                   EventData(animationParameter),
                   [](const EventData &ed1, const EventData &ed2)
        {
            return !(ed1 < ed2);
        }
                   );
    }
}

void Model::Display()
{
    scene->clear();

    // Draw points
    for (VPoint *point : *vertices)
    {
        DrawPoint(point);
    }

    // Draw Edges
    for (VEdge *edge: *edges)
    {
        DrawLine(edge);

        if (edge->start->x < 0 || edge->start->x > width ||
            edge->start->y < 0 || edge->start->y > height)
        {
            if (edge->end->x < 0 || edge->end->x > width ||
                edge->end->y < 0 || edge->end->y > height)
            {
                //std::cout << "Invalid line" << std::endl;
            }
        }
    }

    // Draw sweeping line
    scene->addLine(0, height - animationParameter,
        width, height - animationParameter);

    std::cout << FindEventData().ly << std::endl;
}
