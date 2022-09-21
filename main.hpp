#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include <bits/stdc++.h>

#include "point.hpp"
#include "triangle.hpp"

#include "gl_framework.hpp"
#include "shader_util.hpp"

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
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

GLfloat xrot = 0, yrot = 0, zrot = 0, rotamount = 0.1;
GLfloat cursor_x = 0, cursor_y = 0, cursor_z = 0, moveamount = N_UNITS, cursor_max = DRAW_MAX, cursor_min = DRAW_MIN;
GLfloat cursor_red = 1, cursor_green = 1, cursor_blue = 0;
bool persp = false;
float color_r=1.0, color_g=1.0, color_b=0.0;
// TODO: rename to cursor_color for better readability
Point current_color(color_r, color_g, color_b);

void insertAtCursor();
void deleteAtCursor();
void updateCursor();
#endif
