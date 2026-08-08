#version 430 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D u_SceneColorTexture;

uniform float u_FogDensity;
uniform vec3 u_FogColor;
uniform float u_water_height;
uniform vec3 u_CamPos;
uniform mat4 u_InvViewProj;

void main() {
  FragColor = vec4(texture(u_SceneColorTexture, TexCoords).rgb, 1.0);
}
