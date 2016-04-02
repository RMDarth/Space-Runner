#version 100
precision mediump int;
precision mediump float;

uniform vec3 lightDif0;
uniform vec4 lightPos0;
uniform vec3 lightSpec0;
uniform vec4 matDif;
uniform vec4 matSpec;
uniform float matShininess;
uniform vec3 camPos;
uniform mat4 iTWMat;
uniform sampler2D diffuseMap;
uniform sampler2D specMap; 
uniform sampler2D normalMap;

varying vec3 oNormal;
varying vec2 oUv0;
varying vec3 oTangent;
varying vec3 oBinormal;

varying vec4 worldPos;

void main()
{
    vec3 lightDir0 = normalize(lightPos0.xyz - (lightPos0.w * worldPos.xyz));
 
    vec4 normalTex = texture2D(normalMap, oUv0);
 
    mat3 rotation = mat3(oTangent, oBinormal, oNormal);
    vec3 objectNormal = rotation * (normalTex.xyz * 2.0 - 1.0); // to object space
	mat3 iTWRot = mat3(iTWMat[0].xyz, iTWMat[1].xyz, iTWMat[2].xyz);
	objectNormal = normalize(iTWRot * objectNormal);
    float diffuse = max(dot(lightDir0, objectNormal), 0.0);
 
    vec3 camDir = normalize(camPos - worldPos.xyz);
    vec3 halfVec = normalize(lightDir0 + camDir);
    float specular = pow(max(dot(objectNormal, halfVec), 0.0), matShininess);
 
    vec4 diffuseTex = texture2D(diffuseMap, oUv0);
    vec4 specTex = texture2D(specMap, oUv0);
 
    vec3 diffuseContrib = (diffuse * lightDif0 * diffuseTex.rgb * matDif.rgb);
    vec3 specularContrib = (specular * lightSpec0 * specTex.rgb * matSpec.rgb);
    vec3 light0C = (diffuseContrib + specularContrib);
 
    gl_FragColor = diffuseTex * 0.1 + vec4(light0C, diffuseTex.a);
}