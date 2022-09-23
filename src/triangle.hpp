#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include "point.hpp"
#include <string>
#include <sstream>
#include <unordered_set>
struct Triangle {
    std::string tri_str;
    Point p1, p2, p3;
    Point c;
    Triangle(Point p1, Point p2, Point p3, Point c);
    struct Triangle withColor(Point color);
    bool operator<(const struct Triangle &t) const;
    bool operator==(const struct Triangle &t) const;
    struct HashTriangle {
        size_t operator()(const Triangle &t) const {
            return std::hash<std::string>()(t.tri_str);
        }
    };
};
typedef struct Triangle Triangle;
#endif
