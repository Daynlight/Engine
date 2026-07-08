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
#include "Utils/config.h"
#include "Utils/Logger.h"



namespace UW {
class ScriptSerialization {
public:
  ScriptSerialization() = default;
  ~ScriptSerialization() = default;
  
#ifndef PRODUCTION
  void save(const std::string& script_name, const std::string& source);
  std::string load(const std::string& shader_name);
#endif

};
}; // namespace UW
