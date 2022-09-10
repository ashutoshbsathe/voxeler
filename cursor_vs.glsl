#version 330

in vec3 vp;

uniform mat4 uModelViewProjectMatrix;

void main () 
{
  vec4 position = vec4(vp, 1.0);
  gl_Position = uModelViewProjectMatrix * position;
}
