#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 uvs;

out vec2 TexCoords;
out vec3 FragPosition;
out vec3 v_cameraPos;
out vec2 atlasSizes;

uniform vec3 cameraPosition;
uniform mat4 projection;
uniform mat4 view;
uniform vec2 window_size;
uniform vec2 sizes;
uniform vec2 repeate;
uniform vec2 atlas_size;



void main(){
  vec3 localCamPos = cameraPosition;
  localCamPos.z = -1.0f;

  vec3 worldPos = aPos + localCamPos;

  vec4 pos = projection * view * vec4(worldPos, 1.0f);
    
  pos.xy *= repeate.xy;

  atlasSizes = atlas_size;
  v_cameraPos = cameraPosition; 
    
  FragPosition = worldPos; 
  TexCoords = (uvs / atlas_size) * repeate; 

  gl_Position = pos;
}


