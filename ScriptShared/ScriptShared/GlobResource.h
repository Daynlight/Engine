// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once


#include <string>
#include <vector>
#include <unordered_map>
#include <variant>



#define GLM_ENABLE_EXPERIMENTAL
#ifdef BUILDING_SCRIPT_DLL
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/quaternion.hpp"
#else
#include "Renderer.h"
#include "../vendor/glm/glm/gtc/quaternion.hpp"
#endif



#ifdef BUILDING_SCRIPT_DLL
#include "InputData.h"
namespace CW{
class InoutData;
};
#else
#include "Renderer.h"
#endif



namespace UW{
inline constexpr const char* globResourceName[] = {"int", "float", "bool", "vec2", "vec3", "str"};
using GlobResourceName = std::variant<int, float, bool, glm::vec2, glm::vec3, std::string>;



class GlobResource{
public:
  std::unordered_map<std::string, GlobResourceName> resources;
  std::string WINDOW_TITLE = "Under the Water (Daniel Stodulski)";
  float FIXED_HZ = 60.0f;
  unsigned int VSYNC = 0;
  
  const CW::Renderer::InputData* input_data;
  
  static GlobResource& get() {
    static GlobResource instance = GlobResource();
    return instance;
  };

  GlobResource(const GlobResource&) = delete;
  GlobResource& operator=(const GlobResource&) = delete;
  GlobResource(GlobResource&&) = delete;
  GlobResource& operator=(GlobResource&&) = delete;


private:
  GlobResource() = default;
  ~GlobResource() = default;
  
};
};
