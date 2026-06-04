#pragma once

#include <string>


namespace TestingCubeShader{
inline std::string vertex = R"(#version 430 core

layout(location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

uniform mat4 model;

void main(){
  vec4 pos = projection * view * model * vec4(aPos, 1.0);

  gl_Position = pos;
}
)";

inline std::string fragment = R"(#version 430 core

out vec4 FragColor;

void main(){
  FragColor = vec4(1.0);
}
)";
};