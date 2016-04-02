#version 100
precision mediump int;
precision mediump float;

uniform sampler2D diffuseMap;
uniform vec4 color;
uniform float width;
uniform float height;

varying vec2 oUv0;

void main()
{
    float size = width * height; 
    float index = (1.0 - color.a) * size;
    float y = floor(index / width);
    float x = floor(index - width * y);
    
    float startU = x / width;
    float endU = (x + 1.0) / width;
    float u = startU + (endU - startU) * oUv0.x;
    
    float startV = y / height;
    float endV = (y + 1.0) / height;
    float v = startV + (endV - startV) * oUv0.y;
    
    gl_FragColor = texture2D(diffuseMap, vec2(u, v)) * vec4(color.rgb, 1.0);
}