#version 100

precision highp float;

attribute float  pow_attrib;
attribute float  bin_attrib;

uniform mat4 ortho;


void main()
{
    gl_Position = ortho * vec4(bin_attrib, pow_attrib, 0.0, 1.0);
}
