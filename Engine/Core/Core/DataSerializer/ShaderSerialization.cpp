// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "ShaderSerialization.h"

#ifdef PRODUCTION
#include <cmrc/cmrc.hpp>
CMRC_DECLARE(GameData);
#endif



#ifndef PRODUCTION
void UW::ShaderSerialization::save(const std::string &shader_name, GLuint type, const std::string& source, std::unordered_map<std::string, CW::Renderer::Shader>& shaders){
  Engine::Utils::Logger::get().info("ShaderSerialization", "Saving shader: " + shader_name + " type=" + std::to_string(type));
  std::string local_path = Engine::Config::GAME_DATA_FOLDER + Engine::Config::ASSETS_FOLDER + Engine::Config::SHADERS_FOLDER + shader_name + "/" + Engine::Config::SHADER_TYPE_TO_NAME[type];
  
  try {
    std::filesystem::path p(local_path);
    if (p.has_parent_path())
      std::filesystem::create_directories(p.parent_path());
  } catch (const std::filesystem::filesystem_error& e) {
    Engine::Utils::Logger::get().erro("ShaderSerialization", "Filesystem error while creating directories - " + std::string(e.what()));
    return;
  };

  std::ofstream outFile(local_path);
  if (!outFile.is_open()) {
    Engine::Utils::Logger::get().erro("ShaderSerialization", "Failed to open file for saving - " + local_path);
    return;
  };

  outFile << source << "\n";
  
  outFile.close();
  Engine::Utils::Logger::get().info("ShaderSerialization", "Shader saved: " + shader_name);
};
#endif



void UW::ShaderSerialization::load(const std::string& shader_name, std::unordered_map<std::string, CW::Renderer::Shader>& shaders){
  Engine::Utils::Logger::get().info("ShaderSerialization", "Loading shader: " + shader_name);
  std::string local_path = Engine::Config::GAME_DATA_FOLDER + Engine::Config::ASSETS_FOLDER + Engine::Config::SHADERS_FOLDER + shader_name;
  CW::Renderer::Shader shader;

  for(const auto& [type_name, type_enum] : Engine::Config::SHADER_NAME_TO_TYPE){
    std::string file_path = local_path + "/" + type_name;

#ifndef PRODUCTION
    std::ifstream inFile(file_path);
    if (inFile.is_open()) {
      std::string source((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
      shader.setShader(source, type_enum);
      continue;
    }
#else
    try {
      auto fs = cmrc::GameData::get_filesystem();
      if (fs.exists(file_path)) {
        auto file = fs.open(file_path); 
        std::string source(file.begin(), file.end());
        shader.setShader(source, type_enum);
        continue;
      }
    } catch (const std::exception& e) {
      Engine::Utils::Logger::get().warn("ShaderSerialization", "[LoadShader] CMRC Exception: " + std::string(e.what()));
    }
#endif
  };

  if(shader.getRegisterShader().size() != 0){
    shaders[shader_name] = std::move(shader);
    shaders[shader_name].compile();
    Engine::Utils::Logger::get().info("ShaderSerialization", "Shader loaded: " + shader_name);
  } else {
    Engine::Utils::Logger::get().info("ShaderSerialization", "No shader source found for: " + shader_name);
  };
};



void UW::ShaderSerialization::loadAll(std::unordered_map<std::string, CW::Renderer::Shader>& shaders) {
  Engine::Utils::Logger::get().info("ShaderSerialization", "Scanning and loading all shaders...");
  
  std::string root_path = Engine::Config::GAME_DATA_FOLDER + Engine::Config::ASSETS_FOLDER + Engine::Config::SHADERS_FOLDER;
  
  if (!root_path.empty() && root_path.back() == '/') root_path.pop_back();

  try {

#ifndef PRODUCTION
    if (std::filesystem::exists(root_path) && std::filesystem::is_directory(root_path)) {
      for (const auto& entry : std::filesystem::directory_iterator(root_path)) {
        if (entry.is_directory()) {
          load(entry.path().filename().string(), shaders);
        }
      }
    } else {
      Engine::Utils::Logger::get().erro("ShaderSerialization", "Filesystem - Directory not found: " + root_path);
    }
#else
    auto fs = cmrc::GameData::get_filesystem();
    if (fs.exists(root_path)) {
      for (auto&& entry : fs.iterate_directory(root_path)) {
        if (entry.is_directory()) {
          load(entry.filename(), shaders);
        }
      }
    } else {
      Engine::Utils::Logger::get().erro("ShaderSerialization", "CMRC - Directory not found: " + root_path);
    }
#endif

    Engine::Utils::Logger::get().info("ShaderSerialization", "Finished loading all shaders.");
  } catch (const std::exception& e) {
    Engine::Utils::Logger::get().erro("ShaderSerialization", "[LoadAll] CMRC Exception: " + std::string(e.what()));
  };
};