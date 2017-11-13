#version 100

precision highp float;

uniform sampler2D waterfall;
varying vec2 textcoord;

void main()
{
    gl_FragColor = texture2D(waterfall, textcoord);
}
