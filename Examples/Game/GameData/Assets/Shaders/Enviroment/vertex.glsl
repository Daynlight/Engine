#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 uvs;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPosition;
out vec2 atlasSizes;
out vec2 Repeat;

uniform mat4 projection;
uniform mat4 view;
uniform vec2 window_size;
uniform vec2 sizes;
uniform vec2 repeat;
uniform vec2 atlas_size;

uniform mat4 model;

void main(){
  vec3 scaledPos = vec3(aPos.x * sizes.x * repeat.x / 2, aPos.y * sizes.y * repeat.y / 2, aPos.z);
  vec4 worldPos = model * vec4(scaledPos, 1.0);

  vec4 pos = projection * view * worldPos;
  
  FragPosition = worldPos.xyz;
  TexCoords = uvs;
  atlasSizes = atlas_size;
  Repeat = repeat;

  gl_Position = pos;
}






