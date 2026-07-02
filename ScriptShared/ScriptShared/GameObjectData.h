// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#ifdef BUILDING_SCRIPT_DLL
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/quaternion.hpp"
#else
#include "Renderer.h"
#include "../vendor/glm/glm/gtc/quaternion.hpp"
#endif



#include <string>
#include <vector>
#include <unordered_map>
#include <variant>



namespace UW{
inline constexpr const char* gameObjectParameterTypeName[] = {"int", "float", "bool", "vec2", "vec3", "str"};
using GameObjectParameterType = std::variant<int, float, bool, glm::vec2, glm::vec3, std::string>;



struct GameObjectData{
  std::string name = "";
  std::string mesh = "";
  std::string shader = "";
  std::vector<std::string> textures;
  std::vector<std::string> materials;
  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 rotation = glm::vec3(0.0f);
  glm::vec3 scale = glm::vec3(1.0f);
  bool hidden = false;
  bool culling_on = true;
  bool dont_write_to_depth_mask = false;
  bool gl_depth_lequal = false;

  std::unordered_map<std::string, GameObjectParameterType> parameters;
  std::unordered_map<std::string, GameObjectParameterType> uniforms;
};
};
