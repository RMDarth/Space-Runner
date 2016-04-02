#version 100
precision mediump int;
precision mediump float;

uniform sampler2D diffuseMap;
uniform vec4 color;
varying vec2 oUv0;

void main()
{
	gl_FragColor = texture2D(diffuseMap, oUv0) * color;
}