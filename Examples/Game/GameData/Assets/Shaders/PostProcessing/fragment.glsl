#version 430 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D u_SceneColorTexture;
uniform sampler2D u_SceneDepthTexture;

uniform float u_FogDensity;
uniform vec3 u_FogColor;

uniform float u_water_height;
uniform vec3 u_CamPos;
uniform mat4 u_InvViewProj;

void main() {
  vec3 sceneColor = texture(u_SceneColorTexture, TexCoords).rgb;

  vec3 finalColor = sceneColor;
  FragColor = vec4(finalColor, 1.0);
}

