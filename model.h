#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <math.h>
#include <algorithm>
#include <time.h>

#include "voronoi.h"
#include "vpoint.h"

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

public:

    Model(int w, int h, int n);

    void Init();
};

#endif // MODEL_H
