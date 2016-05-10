#version 100
precision mediump int;
precision mediump float;

uniform sampler2D diffuseMap;
uniform vec4 color;
uniform float tiles;
varying vec2 oUv0;

void main()
{
    gl_FragColor = texture2D(diffuseMap, vec2((oUv0.x + color.a) * tiles, oUv0.y)) * vec4(color.rgb, 1.0);
}