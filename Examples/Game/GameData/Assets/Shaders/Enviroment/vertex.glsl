#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 uvs;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPosition;
out vec4 FragPosLightSpace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 u_LightSpaceMatrix;
uniform vec2 window_size;
uniform vec2 sizes;
uniform vec2 repeate;

uniform mat4 model;

void main(){
  vec3 scaledPos = vec3(aPos.x * sizes.x * repeate.x, aPos.y * sizes.y * repeate.y, aPos.z);
  vec4 worldPos = model * vec4(scaledPos, 1.0);

  FragPosLightSpace = u_LightSpaceMatrix * worldPos;
  vec4 pos = projection * view * worldPos;
  
  FragPosition = worldPos.xyz;
  TexCoords = uvs * repeate;

  gl_Position = pos;
}




