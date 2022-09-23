#ifndef __POINT_H__
#define __POINT_H__
struct Point {
    float x, y, z;
    Point();
    Point(float x, float y, float z);
    bool operator<(const struct Point &p) const;
    bool operator==(const struct Point &p) const;
    Point operator=(const struct Point &p);
    Point operator+(const struct Point &p);
};
typedef struct Point Point;
#endif
