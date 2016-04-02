#version 100
precision mediump int;
precision mediump float;

uniform mat4 wvpMat;

attribute vec4 vertex;
attribute vec4 uv0;

varying vec2 oUv0;

void main()
{
	gl_Position = wvpMat * vertex;
	oUv0 = uv0.xy;
}