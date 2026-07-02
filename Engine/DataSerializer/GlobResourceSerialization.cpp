// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "GlobResourceSerialization.h"

#ifdef PRODUCTION
#include <cmrc/cmrc.hpp>
CMRC_DECLARE(GameData);
#endif



#ifndef PRODUCTION
void UW::GlobResourceSerialization::saveAll() {
  Logger::get().info("GlobResourceSerialization", "Saving resources data");
  try {
    std::filesystem::path p(UW::Config::GAME_DATA_FOLDER + UW::Config::RESOURCES_FILENAME);
    if (p.has_parent_path())
      std::filesystem::create_directories(p.parent_path());
  } catch (const std::filesystem::filesystem_error& e) {
    Logger::get().erro("GlobResourceSerialization", "Filesystem error - " + std::string(e.what()));
    return;
  }

  std::ofstream outFile(UW::Config::GAME_DATA_FOLDER + UW::Config::RESOURCES_FILENAME, std::ios::binary);
  if (!outFile.is_open()) {
    Logger::get().erro("GlobResourceSerialization", "Failed to open file for saving");
    return;
  };

  UW::GlobResourceRecord record;
  record.window_title = UW::GlobResource::get().WINDOW_TITLE;
  record.Fixed_HZ = UW::GlobResource::get().FIXED_HZ;
  record.vsync = UW::GlobResource::get().VSYNC;
    
  outFile << record;
  
  outFile.close();
  Logger::get().info("GlobResourceSerialization", "Glob Resources saved");
};
#endif



void UW::GlobResourceSerialization::loadAll() {
  Logger::get().info("GlobResourceSerialization", "Loading all Resources...");
  try {
    std::string resourcePath = UW::Config::GAME_DATA_FOLDER + UW::Config::RESOURCES_FILENAME;

#ifndef PRODUCTION
    std::ifstream inFile(resourcePath, std::ios::binary);
    
    if (!inFile.is_open()) {
      Logger::get().erro("GlobResourceSerialization", "Failed to open file for loading - " + resourcePath);
      return;
    };
#else
    auto fs = cmrc::GameData::get_filesystem();
    
    if (!fs.exists(resourcePath)) {
      Logger::get().erro("GlobResourceSerialization", "CMRC - File not found - " + resourcePath);
      return;
    };

    auto embeddedFile = fs.open(resourcePath);
    std::string dataStr(embeddedFile.begin(), embeddedFile.end());
    std::stringstream inFile(dataStr);
#endif

    UW::GlobResourceRecord record;
    if (inFile >> record) {
      UW::GlobResource::get().WINDOW_TITLE = record.window_title;
      UW::GlobResource::get().FIXED_HZ = record.Fixed_HZ;
      UW::GlobResource::get().VSYNC = record.vsync;
      

      Logger::get().info("GlobResourceSerialization", "Glob Resources Loaded");
    } else {
      Logger::get().erro("GlobResourceSerialization", "File format is corrupted");
    };

  } catch(const std::exception& e) {
    Logger::get().erro("GlobResourceSerialization", "Exception - " + std::string(e.what()));
  };
};



#ifndef PRODUCTION
std::ostream& UW::operator<<(std::ostream& os, const UW::GlobResourceRecord& record) {
  uint32_t window_title_sz = static_cast<uint32_t>(record.window_title.size());
  os.write(reinterpret_cast<const char*>(&window_title_sz), sizeof(window_title_sz));
  if (window_title_sz > 0) os.write(record.window_title.data(), window_title_sz);

  os.write(reinterpret_cast<const char*>(&record.Fixed_HZ), sizeof(float));
  os.write(reinterpret_cast<const char*>(&record.vsync), sizeof(unsigned int));

  return os;
};
#endif



std::istream& UW::operator>>(std::istream& is, UW::GlobResourceRecord& record) {
  uint32_t window_title_sz = 0;
  if (!is.read(reinterpret_cast<char*>(&window_title_sz), sizeof(window_title_sz))) return is;
  record.window_title.resize(window_title_sz);
  if (window_title_sz > 0) is.read(&record.window_title[0], window_title_sz);

  is.read(reinterpret_cast<char*>(&record.Fixed_HZ), sizeof(float));
  is.read(reinterpret_cast<char*>(&record.vsync), sizeof(unsigned int));

  return is;
};
