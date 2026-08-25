// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "TextureSerialization.h"


#ifdef PRODUCTION
#include <cmrc/cmrc.hpp>
CMRC_DECLARE(GameData);
#endif


namespace fs = std::filesystem;



#ifndef PRODUCTION
void Engine::TextureSerialization::save(const std::string& texture_name, const CW::Renderer::Texture& source) {
  // Engine::Utils::Logger::get().info("ScriptSerialization", "Saving script: " + Engine::Config::TEXTURES_FOLDER + texture_name);
  
  // std::string folder_path = Engine::Config::GAME_DATA_FOLDER + Engine::Config::TEXTURES_FOLDER;
  // std::string file_path = folder_path + texture_name;

  // try {
  //   if (!fs::exists(folder_path)) fs::create_directories(folder_path);

  //   std::ofstream outFile(file_path);
  //   if (!outFile.is_open()) {
  //     Engine::Utils::Logger::get().erro("ScriptSerialization", "Failed to open file: " + file_path);
  //     return;
  //   };

  //   outFile << source;
  //   outFile.close();

  //   Engine::Utils::Logger::get().info("ScriptSerialization", "Script saved: " + file_path);
  // } catch (const fs::filesystem_error& e) {
  //   Engine::Utils::Logger::get().erro("ScriptSerialization", "Filesystem error: " + std::string(e.what()));
  // };
};

#endif


void Engine::TextureSerialization::load(const std::string& texture_name, Engine::Utils::ResourceController<CW::Renderer::Texture>& textures) {
//   std::string file_path = Engine::Config::GAME_DATA_FOLDER + Engine::Config::ASSETS_FOLDER + Engine::Config::TEXTURES_FOLDER + texture_name;

// #ifndef PRODUCTION
//   if (!fs::exists(file_path)) {
//     Engine::Utils::Logger::get().warn("TextureSerialization", "Texture file not found: " + file_path);
//     return;
//   };

//   std::ifstream inFile(file_path);
//   if (!inFile.is_open()) {
//     Engine::Utils::Logger::get().erro("TextureSerialization", "Failed to open file: " + file_path);
//     return;
//   };

//   if (!std::filesystem::is_directory(file_path)) {
//     CW::Renderer::TextureLoader loader = CW::Renderer::TextureLoader(file_path);


//     CW::Renderer::Texture texture_temp = CW::Renderer::Texture(loader.data);
//     textures.emplace_back(texture_name, std::move(texture_temp));

//   }
// #else
//   try {
//     auto fs = cmrc::GameData::get_filesystem();
    
//     if (fs.exists(file_path)) {
//       auto file = fs.open(file_path); 
      
//       const unsigned char* data_ptr = reinterpret_cast<const unsigned char*>(file.begin());
//       CW::Renderer::TextureLoader loader(data_ptr, file.size());


//       CW::Renderer::Texture texture_temp = CW::Renderer::Texture(loader.data);
//       textures.emplace_back(texture_name, std::move(texture_temp));

//       // textures.emplace(texture_name, CW::Renderer::Texture()).first;
//       // textures[texture_name].compile(loader.data);
//     } else {
//       Engine::Utils::Logger::get().warn("TextureSerialization", "Texture file not found in CMRC: " + file_path);
//     }
//   } catch (const std::exception& e) {
//     Engine::Utils::Logger::get().warn("Resources", "[getTexture] CMRC Exception: " + std::string(e.what()));
//   };
// #endif
};



void Engine::TextureSerialization::loadAll(Engine::Utils::ResourceController<CW::Renderer::Texture>& textures) {
  Engine::Utils::Logger::get().info("DataSerializer", "Scanning and loading all textures...");

  std::string root_path = Engine::Config::GAME_DATA_FOLDER + Engine::Config::ASSETS_FOLDER + Engine::Config::TEXTURES_FOLDER;
  if (!root_path.empty() && root_path.back() == '/') root_path.pop_back();

#ifndef PRODUCTION
  try {
    if (std::filesystem::exists(root_path) && std::filesystem::is_directory(root_path)) {
      for (const auto& entry : std::filesystem::recursive_directory_iterator(root_path)) {
        if (entry.is_regular_file()) {
          std::string relative_path = std::filesystem::relative(entry.path(), root_path).string();

          if (textures.exists(relative_path)) continue; 

          std::ifstream file(entry.path(), std::ios::binary | std::ios::ate);
          if (file.is_open()) {
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            std::vector<unsigned char> buffer(size);
            if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
              CW::Renderer::TextureLoader loader(buffer.data(), size);
              
              CW::Renderer::Texture texture_temp(loader.data);
              textures.emplace_back(relative_path, std::move(texture_temp));
                  
              Engine::Utils::Logger::get().info("DataSerializer", "Loaded texture from Disk: " + relative_path);
            }
          }
        }
      }
    } else {
      Engine::Utils::Logger::get().warn("DataSerializer", "Filesystem - Directory not found: " + root_path);
    }
  } catch (const std::filesystem::filesystem_error& e) {
    Engine::Utils::Logger::get().warn("DataSerializer", "[Filesystem] Could not scan local textures folder: " + std::string(e.what()));
  }
#else
  try {
    auto fs = cmrc::GameData::get_filesystem();
    
    if (fs.exists(root_path)) {
      std::function<void(const std::string&)> scan_cmrc_dir = [&](const std::string& current_dir) {
        for (auto&& entry : fs.iterate_directory(current_dir)) {
          std::string full_path = current_dir + "/" + entry.filename();

          if (entry.is_file()) {
            std::string relative_path = full_path.substr(root_path.length() + 1);

            if (textures.exists(relative_path)) continue;

            auto file = fs.open(full_path); 
            const unsigned char* data_ptr = reinterpret_cast<const unsigned char*>(file.begin());
            
            CW::Renderer::TextureLoader loader(data_ptr, file.size());

            CW::Renderer::Texture texture_temp(loader.data);
            textures.emplace_back(relative_path, std::move(texture_temp));
            
            Engine::Utils::Logger::get().info("DataSerializer", "Loaded texture from CMRC: " + relative_path);
          } else if (entry.is_directory()) {
            scan_cmrc_dir(full_path);
          }
        }
      };

      scan_cmrc_dir(root_path);
    } else {
      Engine::Utils::Logger::get().warn("DataSerializer", "CMRC - Directory not found: " + root_path);
    }
  } catch (const std::exception& e) {
    Engine::Utils::Logger::get().warn("DataSerializer", "[CMRC] Could not scan textures folder: " + std::string(e.what()));
  }
#endif

  Engine::Utils::Logger::get().info("DataSerializer", "Finished loading all textures.");
}