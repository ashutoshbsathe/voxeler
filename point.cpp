#include "point.hpp"
Point::Point() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}
Point::Point(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
bool Point::operator<(const struct Point &p) const {
    if(this->x == p.x) {
        if(this->y == p.y) {
            return this->z < p.z;
        }
        else {
            return this->y < p.y;
        }
    }
    else {
        return this->x < p.x;
    }
}
bool Point::operator==(const struct Point &p) const {
    return this->x == p.x && this->y == p.y && this->z == p.z;
}
Point Point::operator=(const struct Point &p) {
    this->x = p.x;
    this->y = p.y;
    this->z = p.z;
    return *this;
}
Point Point::operator+(const struct Point &p) {
    Point ret;
    ret.x = this->x + p.x;
    ret.y = this->y + p.y;
    ret.z = this->z + p.z;
    return ret;
}
