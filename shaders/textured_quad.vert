#version 100

precision highp float;

attribute vec2 vec;
uniform float offset;
varying vec2 textcoord;

const vec2 norm = vec2(0.5);

void main()
{
   textcoord = vec * norm + norm + vec2(0, offset);
   gl_Position = vec4(vec, 0.0, 1.0);
}
