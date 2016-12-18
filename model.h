#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <math.h>
#include <algorithm>
#include <time.h>
#include <QGraphicsScene>
#include <vector>

#include "voronoi.h"
#include "vpoint.h"

#define POINT_SIZE			5.0
#define PARABOLA_DRAW_STEP	5.0
#define PARABOLA_PRECISION	5.0

using namespace vor;

class Model
{
private:

    Voronoi *voronoi;
    Vertices *vertices;
    Edges *edges;

    double width;
    double height;

    double numOfPoints;
    double animationParameter;

    EventsData eventsData;

    QGraphicsScene *scene;

    EventData   &FindEventData();

public:

    Model(int w, int h);

    double Width() const;

    double Height() const;

    double GetYFromAP(double ap);

    double ModelToDisplayX(double x);

    double ModelToDisplayY(double y);

    void SetNumOfPoints(int n);

    void SetAnimationParameter(double value);

    double AnimationParameter() const;

    QGraphicsScene *Scene();

    void Clear();

    void Init();

    void Display();

    void DrawPoint(VPoint *point, bool isSpecial);

    void DrawLine(const VEdge *edge);

    void DrawLine(double startX, double startY, double endX, double endY);
};

#endif // MODEL_H
