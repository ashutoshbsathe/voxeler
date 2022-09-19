#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include "point.hpp"
struct Triangle {
    Point p1, p2, p3;
	Point c;
    Triangle(Point p1, Point p2, Point p3, Point c);
	struct Triangle changeColor(Point color);
    bool operator<(const struct Triangle &t) const;
    bool operator==(const struct Triangle &t) const;
};
typedef struct Triangle Triangle;
#endif
