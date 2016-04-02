#version 100
precision mediump int;
precision mediump float;

uniform sampler2D diffuseMap;

varying vec2 oUv0;
varying vec4 oColor;

void main()
{
	gl_FragColor = texture2D(diffuseMap, oUv0) * oColor;
}