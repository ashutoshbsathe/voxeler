#version 330
#ifndef MAX_N_SIZE 
#define MAX_N_SIZE 102
#endif

in vec2 vp;

uniform float grid_offsets[MAX_N_SIZE];

void main () 
{
  gl_Position = vec4 (grid_offsets[gl_InstanceID + 1], vp, 1.0);
}
