#version 100
precision highp int;
precision highp float;

uniform mat4 wvpMat;

attribute vec4 vertex;
attribute vec4 uv0;
attribute vec4 colour;

varying vec2 oUv0;
varying vec4 oColor;

void main()
{
	gl_Position = wvpMat * vertex;
	oUv0 = uv0.xy;
	oColor = colour;
}