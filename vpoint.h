#ifndef VPOINT_H
#define VPOINT_H

/*
    A structure that stores 2D point
 */

struct VPoint
{
public:

  double x, y;

  /*
      Constructor for structure; x, y - coordinates
   */

  VPoint(double nx, double ny)
  {
    x = nx;
    y = ny;
  }
};

#endif // VPOINT_H
