// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include "Renderer.h"

#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <unordered_map>

#ifdef PRODUCTION
#include <cmrc/cmrc.hpp>
#endif

#include "Utils/config.h"
#include "Utils/Logger.h"



namespace UW {
class ShaderSerialization {
public:
  ShaderSerialization() = default;
  ~ShaderSerialization() = default;
  
#ifndef PRODUCTION
  void save(const std::string& shader_name, GLuint type, const std::string& source, std::unordered_map<std::string, CW::Renderer::Shader>& shaders);
#endif
  void load(const std::string& shader_name, std::unordered_map<std::string, CW::Renderer::Shader>& shaders);

  void loadAll(std::unordered_map<std::string, CW::Renderer::Shader>& shaders);
};
}; // namespace UW
