#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include "point.hpp"
struct Triangle {
    Point p1, p2, p3;
    Triangle(Point p1, Point p2, Point p3);
    bool operator<(const struct Triangle &t) const;
    bool operator==(const struct Triangle &t) const;
};
typedef struct Triangle Triangle;
#endif
