// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once

#include "../../build/_deps/cwindow-src/CWindow/vendor/glm/glm/glm.hpp"
#include "../../build/_deps/cwindow-src/CWindow/vendor/glm/glm/gtc/quaternion.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>



namespace UW{
inline constexpr const char* globResourceName[] = {"int", "float", "bool", "vec2", "vec3", "str"};
using GlobResourceName = std::variant<int, float, bool, glm::vec2, glm::vec3, std::string>;



class GlobResource{
public:
  std::unordered_map<std::string, GlobResourceName> resources;
  std::string WINDOW_TITLE = "Under the Water (Daniel Stodulski)";
  float FIXED_HZ = 60.0f;
  unsigned int VSYNC = 0;

  // key events
  // mouse events
  
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
