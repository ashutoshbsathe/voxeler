#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include <bits/stdc++.h>

#include "point.hpp"
#include "triangle.hpp"
#include "model.hpp"

#include "gl_framework.hpp"
#include "shader_util.hpp"

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// N_CELLS x N_CELLS x N_CELLS cubes
#define N_CELLS 100
// each cube is N_UNITS x N_UNITS x N_UNITS
#define N_UNITS 5

#define CURSOR_PADDING (1.15)
#define VIEW_PADDING (1.5)

#define DRAW_MIN (-(N_CELLS*N_UNITS)/2)
#define DRAW_MAX ((N_CELLS*N_UNITS)/2)

bool persp = false, draw_grid = true, isInspectMode = false;
float xrot = 0, yrot = 0, zrot = 0, rotamount = glm::pi<float>() / 12; // 15 degrees

float cursor_x = 0, cursor_y = 0, cursor_z = 0, moveamount = N_UNITS, cursor_max = DRAW_MAX, cursor_min = DRAW_MIN;
float cursor_r = 1.0, cursor_g = 1.0, cursor_b = 0.0;
Point cursor_color(cursor_r, cursor_g, cursor_b);

unsigned long max_num_triangles = 2 * (N_CELLS) * (N_CELLS + 1) * (2 * N_CELLS + 1), max_num_cubes = N_CELLS;

void insertAtCursor();
void deleteAtCursor();
void updateCursor();
#endif
