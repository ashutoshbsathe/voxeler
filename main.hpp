#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#define DRAW_MIN (-0.9)
#define DRAW_MAX (0.9)

#include "gl_framework.hpp"
#include "shader_util.hpp"

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// N_CELLS x N_CELLS x N_CELLS cubes
#define N_CELLS 10
// each cube is N_UNITS x N_UNITS x N_UNITS
#define N_UNITS 5

GLfloat xrot = 0, yrot = 0, zrot = 0;

#endif
