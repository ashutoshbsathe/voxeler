#version 330
#ifndef MAX_N_CELLS
#define MAX_N_CELLS 102
#endif

in vec2 vp;

uniform mat4 uModelViewProjectMatrix;
uniform float grid_offsets[MAX_N_CELLS];

void main () 
{
  int n = int(grid_offsets[0]);
  float offset = grid_offsets[(gl_InstanceID) % (n+1) + 1];
  vec4 position;
  if(gl_InstanceID >= 2*(n+1)) {
    position = vec4(offset, vp.x, vp.y, 1.0);
  }
  else if(gl_InstanceID >= (n+1)) {
    position = vec4(vp.y, offset, vp.x, 1.0);
  }
  else {
    position = vec4(vp.x, vp.y, offset, 1.0);
  }
  gl_Position = uModelViewProjectMatrix * position;
}
