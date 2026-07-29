// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "LightsSerialization.h"

#ifdef PRODUCTION
#include <cmrc/cmrc.hpp>
CMRC_DECLARE(GameData);
#endif



#ifndef PRODUCTION
void Engine::LightsSerialization::save(const std::string& name, const Engine::Core::Light& light) {

};
#endif



void Engine::LightsSerialization::load(const std::string& name, Engine::Core::Light& light) {

};



#ifndef PRODUCTION
void Engine::LightsSerialization::saveAll(Engine::Core::Lights& lights) {
  Engine::Utils::Logger::get().info("LightsSerialization", "Saving all lights...");
  try {
    std::filesystem::path p(Engine::Config::GAME_DATA_FOLDER + Engine::Config::LIGHTS_FILENAME);
    if (p.has_parent_path())
      std::filesystem::create_directories(p.parent_path());
  } catch (const std::filesystem::filesystem_error& e) {
    Engine::Utils::Logger::get().erro("LightsSerialization", "Filesystem error - " + std::string(e.what()));
    return;
  }

  std::ofstream outFile(Engine::Config::GAME_DATA_FOLDER + Engine::Config::LIGHTS_FILENAME, std::ios::binary);
  if (!outFile.is_open()) {
    Engine::Utils::Logger::get().erro("LightsSerialization", "Failed to open file for saving");
    return;
  }

  unsigned int size = lights.size();

  outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

  for(int i = 0; i < lights.size(); i++){
    Engine::Core::Light light = lights.get(i);
    Engine::LightsRecord record;
      
    record.position = light.position;
    record.color = light.color;
    record.strength = light.strength;

    outFile << record;
  };

  outFile.close();
  Engine::Utils::Logger::get().info("LightsSerialization", "All lights have been saved");
};
#endif



void Engine::LightsSerialization::loadAll(Engine::Core::Lights& lights) {
  Engine::Utils::Logger::get().info("LightsSerialization", "Loading all lights...");
  try {
    std::string resourcePath = Engine::Config::GAME_DATA_FOLDER + Engine::Config::LIGHTS_FILENAME;

#ifndef PRODUCTION
    std::ifstream inFile(resourcePath, std::ios::binary);
    
    if (!inFile.is_open()) {
      Engine::Utils::Logger::get().erro("LightsSerialization", "Failed to open file for loading - " + resourcePath);
      return;
    }
#else
    auto fs = cmrc::GameData::get_filesystem();
    
    if (!fs.exists(resourcePath)) {
      Engine::Utils::Logger::get().erro("LightsSerialization", "CMRC - File not found - " + resourcePath);
      return;
    }

    auto embeddedFile = fs.open(resourcePath);
    std::string dataStr(embeddedFile.begin(), embeddedFile.end());
    std::stringstream inFile(dataStr);
#endif

    unsigned int lightCount = 0;
    inFile.read(reinterpret_cast<char*>(&lightCount), sizeof(lightCount));

    lights.clear();

    for (unsigned int i = 0; i < lightCount; ++i) {
      Engine::LightsRecord record;
      if (inFile >> record) {
        Engine::Core::Light light(record.position, record.color, record.strength);
        lights.emplace_back(light);
      } else {
        Engine::Utils::Logger::get().erro("LightsSerialization", "File format corrupted at index " + std::to_string(i));
        break;
      };
    };

    lights.compile();

    Engine::Utils::Logger::get().info("LightsSerialization", "All lights have been loaded");
  } catch(const std::exception& e) {
    Engine::Utils::Logger::get().erro("LightsSerialization", "Exception - " + std::string(e.what()));
  };
};



#ifndef PRODUCTION
std::ostream& Engine::operator<<(std::ostream& os, const Engine::LightsRecord& record) {
  os.write(reinterpret_cast<const char*>(&record.position), sizeof(glm::vec3));
  os.write(reinterpret_cast<const char*>(&record.color), sizeof(glm::vec3));
  os.write(reinterpret_cast<const char*>(&record.strength), sizeof(float));

  return os;
};
#endif



std::istream& Engine::operator>>(std::istream& is, Engine::LightsRecord& record) {
  is.read(reinterpret_cast<char*>(&record.position), sizeof(glm::vec3));
  is.read(reinterpret_cast<char*>(&record.color), sizeof(glm::vec3));
  is.read(reinterpret_cast<char*>(&record.strength), sizeof(float));

  return is;
};
