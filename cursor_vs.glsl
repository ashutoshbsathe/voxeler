#version 330

in vec3 vPosition;
in vec3 vColor;
out vec3 color;

uniform vec3 cursor_offset;
uniform mat4 uModelViewProjectMatrix;

void main () 
{
  vec4 position = vec4(vPosition + cursor_offset, 1.0);
  gl_Position = uModelViewProjectMatrix * position;
  color = vColor;
}
