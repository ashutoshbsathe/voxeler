#version 330

in vec3 vPosition;
in vec3 vColor;
out vec3 color;

uniform mat4 uModelViewProjectMatrix;

void main () 
{
  vec4 position = vec4(vPosition, 1.0);
  gl_Position = uModelViewProjectMatrix * position;
  color = vColor;
}
