// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <unordered_map>

#ifdef PRODUCTION
#include <cmrc/cmrc.hpp>
#endif

#include "Renderer.h"
#include "config.h"
#include "Utils/Logger.h"



namespace UW {
class ShaderSerialization {
public:
  ShaderSerialization() = default;
  ~ShaderSerialization() = default;
  
#ifndef PRODUCTION
  void save(const std::string& shader_name, GLuint type);
#endif
  void load(const std::string& shader_name);

  void loadAll();
};
}; // namespace UW
