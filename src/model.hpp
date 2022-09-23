#ifndef __MODEL_H__
#define __MODEL_H__
#include "point.hpp"
#include "triangle.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <unordered_set>
#include <chrono>
#define DEBUG_MODEL false

// Sorted by lexicographic order of <Point> by default !
extern std::map<Point, Point> model;

extern float *model_triangle_list;
extern float *model_triangle_colors;

extern unsigned long num_triangles;

extern unsigned long max_num_triangles, max_num_cubes;
extern float moveamount, cursor_min, cursor_max, cursor_x, cursor_y, cursor_z, cursor_r, cursor_g, cursor_b;
extern Point cursor_color;
extern void insertAtCursor();

void printTriangleList();

void printModel();

std::pair<std::vector<Triangle>, std::vector<Triangle>> trianglesAt(Point c, Point cubeColor);

void updateTrianglesList(std::vector<Triangle> to_add, std::vector<Triangle> to_remove);

void insertAt(float x, float y, float z, Point cubeColor);

void deleteAt(float x, float y, float z);

void saveModelToFile(std::string fname);

void readModelFromFile(std::string fname);
#endif
