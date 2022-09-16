#include "triangle.hpp"
Triangle::Triangle(Point p1, Point p2, Point p3) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
}
bool Triangle::operator<(const struct Triangle &t) const {
    if(this->p1 == t.p1) {
        if(this->p2 == t.p2) {
            return this->p3 < t.p3;
        }
        else {
            return this->p2 < t.p2;
        }
    }
    else {
        return this->p1 < t.p1;
    }
}
bool Triangle::operator==(const struct Triangle &t) const {
    return this->p1 == t.p1 && this->p2 == t.p2 && this->p3 == t.p3;
}
