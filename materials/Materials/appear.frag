#version 100
precision mediump int;
precision mediump float;

uniform vec4 color;
uniform sampler2D diffuseMap;

varying vec2 oUv0;
varying float alpha;

void main()
{
	vec4 diffuse = texture2D(diffuseMap, oUv0);
	gl_FragColor = vec4(diffuse.rgb, diffuse.a * (1.0 - alpha)) * color;
}