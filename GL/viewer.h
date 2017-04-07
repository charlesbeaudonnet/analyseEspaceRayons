#ifndef VIEWER_H
#define VIEWER_H

#include "point.h"

class Viewer{
    unsigned int width;
    unsigned int height;
  public:
    Viewer(unsigned int w, unsigned int h, char const* title);
    void line(Point p, Point p2);
};

#endif