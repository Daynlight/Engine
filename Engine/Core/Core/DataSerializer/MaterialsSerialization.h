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
#include <cstring>

#include "Utils/Logger.h"
#include "Utils/config.h"
#include "Resources/Materials/Materials.h"



namespace Engine {
struct MaterialsRecord {
  std::string name = "";
  glm::vec3 albedo = glm::vec3(1.0f);
  float metallic = 0.0f;
  float roughness = 1.0f;
  glm::vec3 emission_color = glm::vec3(0.0f);
  float emission_strength = 0.0f;
  float ambient_occlusion = 1.0f;

  friend std::ostream& operator<<(std::ostream& os, const MaterialsRecord& record);
  friend std::istream& operator>>(std::istream& is, MaterialsRecord& record);
};



class MaterialsSerialization {
public:
  MaterialsSerialization() = default;
  ~MaterialsSerialization() = default;

#ifndef PRODUCTION
  void save(const Engine::Core::Material& material);
#endif
  void load(Engine::Core::Material& material);

#ifndef PRODUCTION
  void saveAll(Engine::Core::Materials& materials);
#endif
  void loadAll(Engine::Core::Materials& materials);

private:
#ifndef PRODUCTION
  friend std::ostream& operator<<(std::ostream& os, const MaterialsRecord& record);
#endif
  friend std::istream& operator>>(std::istream& is, MaterialsRecord& record);
};
}; // namespace Engine
