#version 100

precision mediump float;

attribute float pow;
attribute float bin;
uniform float line;
uniform mat4 ortho;

varying float norm_pow;

void main()
{
   norm_pow = smoothstep(-100., 0., pow);
   gl_Position = ortho * vec4(bin, line, 0, 1.0);
   gl_PointSize = 1.0;
}
