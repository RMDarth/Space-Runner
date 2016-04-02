#version 100
precision mediump int;
precision mediump float;

uniform sampler2D diffuseMap;
varying vec2 oUv0;
varying vec4 oColor;
uniform float width;
uniform float height;

void main()
{
    float size = width * height; 
    float index = (1.0 - oColor.a) * size;
    float y = floor(index / width);
    float x = floor(index - width * y);
    
    float startU = x / width;
    float endU = (x + 1.0) / width;
    float u = startU + (endU - startU) * oUv0.x;
    
    float startV = y / height;
    float endV = (y + 1.0) / height;
    float v = startV + (endV - startV) * oUv0.y;
    
    gl_FragColor = texture2D(diffuseMap, vec2(u, v));
}