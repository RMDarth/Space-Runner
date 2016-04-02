#version 100
precision mediump int;
precision mediump float;

uniform mat4 wvpMat;
attribute vec4 vertex;

void main()
{
	gl_Position = wvpMat * vertex;
}