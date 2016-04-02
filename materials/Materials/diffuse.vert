#version 100
precision mediump int;
precision mediump float;

uniform mat4 vpMat;
uniform vec4 boneMatrices[72];
uniform mat4 invWMat;

attribute vec4 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 binormal;
attribute vec4 uv0;
attribute float blendIndices;

varying vec3 oNormal;
varying vec2 oUv0;
varying vec3 oTangent;
varying vec3 oBinormal;

varying vec4 worldPos;
 
void main()
{
	int idx = int(blendIndices) * 3;
	mat4 worldMatrix;
	worldMatrix[0] = boneMatrices[idx];
	worldMatrix[1] = boneMatrices[idx + 1];
	worldMatrix[2] = boneMatrices[idx + 2];
	worldMatrix[3] = vec4(0.0);
	
	worldPos = vec4((vertex * worldMatrix).xyz, 1.0);
	gl_Position = vpMat * worldPos;
	
	oUv0 = uv0.xy;
	
	oNormal = normal;
	oTangent = tangent;
	oBinormal = binormal;

}