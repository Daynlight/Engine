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

#include "Utils/config.h"
#include "Utils/Logger.h"
#include "Utils/Resource/ResourceController.h"



namespace Engine {
class TextureSerialization {
public:
  TextureSerialization() = default;
  ~TextureSerialization() = default;
  
#ifndef PRODUCTION
  void save(const std::string& texture_path, const CW::Renderer::Texture& source);
#endif
  void load(const std::string& texture_path, Engine::Utils::ResourceController<CW::Renderer::Texture>& textures);

  void loadAll(Engine::Utils::ResourceController<CW::Renderer::Texture>& textures);
};
}; // namespace Engine
