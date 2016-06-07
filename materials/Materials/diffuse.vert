#version 100
precision highp int;
precision highp float;

uniform mat4 wMat;
uniform mat4 wvpMat;

attribute vec4 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 binormal;
attribute vec4 uv0;

varying vec3 oNormal;
varying vec2 oUv0;
varying vec3 oTangent;
varying vec3 oBinormal;

varying vec4 worldPos;
 
void main()
{
	gl_Position = wvpMat * vertex;
	worldPos = wMat * vertex;
	
	oUv0 = uv0.xy;
	
	oNormal = normal;
	oTangent = tangent;
	oBinormal = binormal;

}