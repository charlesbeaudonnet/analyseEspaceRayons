#ifndef POINT_H
#define POINT_H

class Point{
  public:
    unsigned int x;
    unsigned int y;
    unsigned int z;
    Point(unsigned int x, unsigned int y);
    Point(unsigned int x, unsigned int y, unsigned int z);
};

#endif