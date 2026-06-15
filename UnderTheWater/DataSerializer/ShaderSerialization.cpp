#include "ShaderSerialization.h"

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(GameData);

#include "Resources/Resources.h"



#ifndef PRODUCTION
void UW::ShaderSerialization::save(const std::string &shader_name, GLuint type){
  Logger::get().info("ShaderSerialization", "Saving shader: " + shader_name + " type=" + std::to_string(type));
  std::string local_path = UW::Config::GAME_DATA_FOLDER + UW::Config::ASSETS_FOLDER + UW::Config::SHADERS_FOLDER + shader_name + "/" + UW::Config::SHADER_TYPE_TO_NAME[type];
  
  auto& reg = Resources::get().getShader(shader_name).getRegisterShader();
  if (reg.find(type) == reg.end()) {
    Logger::get().warn("ShaderSerialization", "Shader type source not found in register for: " + shader_name);
    return;
  }
  
  std::string source = reg.at(type).getSource();
  
  try {
    std::filesystem::path p(local_path);
    if (p.has_parent_path())
      std::filesystem::create_directories(p.parent_path());
  } catch (const std::filesystem::filesystem_error& e) {
    Logger::get().erro("ShaderSerialization", "Filesystem error while creating directories - " + std::string(e.what()));
    return;
  };

  std::ofstream outFile(local_path);
  if (!outFile.is_open()) {
    Logger::get().erro("ShaderSerialization", "Failed to open file for saving - " + local_path);
    return;
  };

  outFile << source << "\n";
  
  outFile.close();
  Logger::get().info("ShaderSerialization", "Shader saved: " + shader_name);
};
#endif



void UW::ShaderSerialization::load(const std::string& shader_name) {
  Logger::get().info("ShaderSerialization", "Loading shader: " + shader_name);
  std::string local_path = UW::Config::GAME_DATA_FOLDER + UW::Config::ASSETS_FOLDER + UW::Config::SHADERS_FOLDER + shader_name;
  CW::Renderer::Shader shader;

  for (const auto& config_pair : UW::Config::SHADER_NAME_TO_TYPE) {
    try {
      auto fs = cmrc::GameData::get_filesystem();
      auto file = fs.open(local_path + "/" + config_pair.first); 
      
      std::string shader_source(file.begin(), file.end());
      const GLuint type = config_pair.second;
      
      shader.setShader(shader_source, type);
    } catch (const std::runtime_error& e) {
      Logger::get().warn("ShaderSerialization", "[LoadShader] CMRC Exception: " + std::string(e.what()));
    };
  };

  if (shader.getRegisterShader().size() != 0) {
    Resources::get().shaders.emplace_back(shader_name, std::move(shader));
    Logger::get().info("ShaderSerialization", "Shader loaded: " + shader_name);
  } else {
    Logger::get().info("ShaderSerialization", "No shader source found for: " + shader_name);
  };
};
