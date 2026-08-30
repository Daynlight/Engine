// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include "CWindow/Renderer/OpenGL/Renderer.h"

#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <unordered_map>

#ifdef PRODUCTION
#include <cmrc/cmrc.hpp>
#endif

#include "Utils/Resource/ResourceController.h"
#include "Utils/config.h"
#include "Utils/Logger.h"



namespace Engine {
class ShaderSerialization {
public:
  ShaderSerialization() = default;
  ~ShaderSerialization() = default;
  
#ifndef PRODUCTION
  void save(const std::string& shader_name, GLuint type, const std::string& source, Engine::Utils::ResourceController<CW::Renderer::Shader>& shaders);
#endif
  void load(const std::string& shader_name, Engine::Utils::ResourceController<CW::Renderer::Shader>& shaders);

  void loadAll(Engine::Utils::ResourceController<CW::Renderer::Shader>& shaders);
};
}; // namespace Engine
