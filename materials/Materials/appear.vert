#version 100
precision mediump int;
precision mediump float;

uniform mat4 wvpMat;
uniform vec2 appearRange;

attribute vec4 vertex;
attribute vec4 uv0;

varying float alpha;
varying vec2 oUv0;

void main()
{
    gl_Position = wvpMat * vertex;
    oUv0 = uv0.xy;
    alpha = smoothstep(appearRange.x, appearRange.y, gl_Position.z);
}