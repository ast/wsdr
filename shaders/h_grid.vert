#version 100

precision mediump float;

attribute vec2 vec_attrib;
uniform mat4 ortho;

void main()
{
   gl_Position = ortho * vec4(vec_attrib, 0.0, 1.0);
}
