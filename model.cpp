#include "model.h"

Model::Model(int w, int h, int n) :
    width(w),
    height(h),
    numOfPoints(n)
{
    voronoi = new Voronoi();
    vertices = new Vertices();

    srand((unsigned)time(NULL));
}

void Model::Init()
{
    for (int i = 0; i < numOfPoints; i++)
    {
        VPoint *vPoint = new VPoint(width * (double)rand() / (double)RAND_MAX,
                height * (double)rand() / (double)RAND_MAX);
        vertices->push_back(vPoint);
    }

    edges = voronoi->GetEdges(vertices, (int)width, (int)height);
    std::cout << "Voronoi diagram done!" << std::endl;

    for (Edges::iterator i = edges->begin(); i != edges->end(); ++i)
    {
        if ((*i)->start == 0)
        {
            std::cout << "Beggining missing!" << std::endl;
            continue;
        }

        if ((*i)->end == 0)
        {
            std::cout << "End missing!" << std::endl;
            continue;
        }
    }
}
