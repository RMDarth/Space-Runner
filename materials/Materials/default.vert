#version 100
precision mediump int;
precision mediump float;

uniform mat4 wMat;
uniform mat4 wvpMat;

attribute vec4 vertex;
attribute vec3 normal;

attribute vec4 uv0;

varying vec3 oNormal;
varying vec2 oUv0;


varying vec4 worldPos;
 
void main()
{
	gl_Position = wvpMat * vertex;
	worldPos = wMat * vertex;

	oNormal = normal;
	oUv0 = uv0.xy;
}