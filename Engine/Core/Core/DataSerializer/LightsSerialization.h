// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once

#include "Renderer.h"

#include <string>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <filesystem>

#include "Utils/Logger.h"
#include "Utils/config.h"
#include "Resources/Lights/Lights.h"



namespace Engine {
struct LightsRecord {
  glm::vec3 position;
  glm::vec3 color;
  float strength;

  friend std::ostream& operator<<(std::ostream& os, const LightsRecord& record);
  friend std::istream& operator>>(std::istream& is, LightsRecord& record);
};



class LightsSerialization {
public:
  LightsSerialization() = default;
  ~LightsSerialization() = default;

#ifndef PRODUCTION
  void save(const std::string& name, const Engine::Core::Light& light);
#endif
  void load(const std::string& name, Engine::Core::Light& light);

#ifndef PRODUCTION
  void saveAll(Engine::Core::Lights& lights);
#endif
  void loadAll(Engine::Core::Lights& lights);

private:
#ifndef PRODUCTION
  friend std::ostream& operator<<(std::ostream& os, const LightsRecord& record);
#endif
  friend std::istream& operator>>(std::istream& is, LightsRecord& record);
};
}; // namespace Engine
