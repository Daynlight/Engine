// Help me I'am Under The Water
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
class TextureSerialization {
public:
  TextureSerialization() = default;
  ~TextureSerialization() = default;
  
#ifndef PRODUCTION
  void save(const std::string& texture_path, const CW::Renderer::Texture& source);
#endif
  void load(const std::string& texture_path, std::unordered_map<std::string, CW::Renderer::Texture>& textures);

  void loadAll(std::unordered_map<std::string, CW::Renderer::Texture>& textures);
};
}; // namespace UW
