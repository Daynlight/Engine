#version 430 core

layout(location = 0) in vec4 aPos;

uniform mat4 model;

void main(){
  gl_Position = model * aPos;
}

