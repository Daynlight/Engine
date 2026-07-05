// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "TextureSerialization.h"



namespace fs = std::filesystem;



#ifndef PRODUCTION
void UW::TextureSerialization::save(const std::string& texture_name, const CW::Renderer::Texture& source) {
  // Logger::get().info("ScriptSerialization", "Saving script: " + UW::Config::TEXTURES_FOLDER + texture_name);
  
  // std::string folder_path = UW::Config::GAME_DATA_FOLDER + UW::Config::TEXTURES_FOLDER;
  // std::string file_path = folder_path + texture_name;

  // try {
  //   if (!fs::exists(folder_path)) fs::create_directories(folder_path);

  //   std::ofstream outFile(file_path);
  //   if (!outFile.is_open()) {
  //     Logger::get().erro("ScriptSerialization", "Failed to open file: " + file_path);
  //     return;
  //   };

  //   outFile << source;
  //   outFile.close();

  //   Logger::get().info("ScriptSerialization", "Script saved: " + file_path);
  // } catch (const fs::filesystem_error& e) {
  //   Logger::get().erro("ScriptSerialization", "Filesystem error: " + std::string(e.what()));
  // };
};

#endif


void UW::TextureSerialization::load(const std::string& texture_name) {
  std::string file_path = UW::Config::GAME_DATA_FOLDER + UW::Config::ASSETS_FOLDER + UW::Config::TEXTURES_FOLDER + texture_name;

  if (!fs::exists(file_path)) {
    Logger::get().warn("TextureSerialization", "Texture file not found: " + file_path);
    return;
  };

  std::ifstream inFile(file_path);
  if (!inFile.is_open()) {
    Logger::get().erro("TextureSerialization", "Failed to open file: " + file_path);
    return;
  };

  CW::Renderer::TextureLoader loader = CW::Renderer::TextureLoader(file_path);
#ifndef PRODUCTION
  if (std::filesystem::exists(file_path) && !std::filesystem::is_directory(file_path)) {
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    if (file.is_open()) {
      std::streamsize size = file.tellg();
      file.seekg(0, std::ios::beg);

      std::vector<unsigned char> buffer(size);
      if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        loader = CW::Renderer::TextureLoader(buffer.data(), size);

        UW::Resources::get().textures.emplace(texture_name, CW::Renderer::Texture()).first;
        UW::Resources::get().textures[texture_name].compile(loader.data);
      };
    } else {
      Logger::get().erro("Resources", "Failed to open texture file: " + file_path);
    };
  };
#else
  try {
    auto fs = cmrc::GameData::get_filesystem();
    
    if (fs.exists(local_path)) {
      auto file = fs.open(local_path); 
      
      const unsigned char* data_ptr = reinterpret_cast<const unsigned char*>(file.begin());
      CW::Renderer::TextureLoader loader(data_ptr, file.size());

      UW::Resources::get().textures.emplace(texture_name, CW::Renderer::Texture()).first;
      UW::Resources::get().textures[texture_name].compile(loader.data);
    };
  } catch (const std::exception& e) {
    Logger::get().warn("Resources", "[getTexture] CMRC Exception: " + std::string(e.what()));
  };
#endif
};



void UW::TextureSerialization::loadAll(){
  Logger::get().info("DataSerializer", "Scanning and loading all textures...");

  std::string root_path = UW::Config::GAME_DATA_FOLDER + UW::Config::ASSETS_FOLDER + UW::Config::TEXTURES_FOLDER;

  if (!root_path.empty() && root_path.back() == '/') root_path.pop_back();

#ifndef PRODUCTION
  try {
    if (std::filesystem::exists(root_path) && std::filesystem::is_directory(root_path)) {
      for (const auto& entry : std::filesystem::directory_iterator(root_path)) {
        if (entry.is_regular_file()) {
          std::string file_name = entry.path().filename().string();

          if (Resources::get().textures.find(file_name) != Resources::get().textures.end()) continue; 

          std::ifstream file(entry.path(), std::ios::binary | std::ios::ate);
          if (file.is_open()) {
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            std::vector<unsigned char> buffer(size);
            if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
              CW::Renderer::TextureLoader loader(buffer.data(), size);
              
              auto it = Resources::get().textures.emplace(file_name, CW::Renderer::Texture()).first;
              it->second.compile(loader.data);
              
              Logger::get().info("DataSerializer", "Loaded texture from Disk: " + file_name);
            };
          };
        };
      };
    } else {
      Logger::get().warn("DataSerializer", "Filesystem - Directory not found: " + root_path);
    }
  } catch (const std::filesystem::filesystem_error& e) {
    Logger::get().warn("DataSerializer", "[Filesystem] Could not scan local textures folder: " + std::string(e.what()));
  };
#else
  try {
    auto fs = cmrc::GameData::get_filesystem();
    
    if (fs.exists(root_path)) {
      for (auto&& entry : fs.iterate_directory(root_path)) {
        if (entry.is_file()) {
          std::string file_name = entry.filename();
          
          if (Resources::get().textures.find(file_name) != Resources::get().textures.end()) continue;

          std::string full_cmrc_path = root_path + "/" + file_name;
          auto file = fs.open(full_cmrc_path); 
          const unsigned char* data_ptr = reinterpret_cast<const unsigned char*>(file.begin());
          
          CW::Renderer::TextureLoader loader(data_ptr, file.size());

          auto it = Resources::get().textures.emplace(file_name, CW::Renderer::Texture()).first;
          it->second.compile(loader.data);
          
          Logger::get().info("DataSerializer", "Loaded texture from CMRC: " + file_name);
        };
      };
    } else {
      Logger::get().warn("DataSerializer", "CMRC - Directory not found: " + root_path);
    }
  } catch (const std::exception& e) {
    Logger::get().warn("DataSerializer", "[CMRC] Could not scan textures folder: " + std::string(e.what()));
  };
#endif

  Logger::get().info("DataSerializer", "Finished loading all textures.");
};