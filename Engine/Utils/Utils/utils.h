// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include "Renderer.h"
#include <filesystem>
#include <string>
#include <regex>
#include <vector>

#ifdef PRODUCTION
#include <cmrc/cmrc.hpp>
#endif



namespace Engine::Utils {
constexpr inline uint32_t hash(uint32_t x) noexcept {
  x = x + 0x45d9f3b;
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = (x >> 16) ^ x;
  return x;
};



template<typename T>
inline void uploadTypedBuffer(CW::Renderer::Mesh& mesh, const std::vector<std::uint8_t>& buffer, unsigned int dimension, unsigned int layout, GLenum type) noexcept {
  if(dimension == 0) return;
  if(buffer.empty()) return;
  if(buffer.size() % sizeof(T) != 0) return;

  size_t count = buffer.size() / sizeof(T);

  std::vector<T> typed;
  typed.resize(count);

  std::memcpy(typed.data(), buffer.data(), buffer.size());
  mesh.setData<T>(std::move(typed), dimension, layout, type);
};



inline void uploadBufferByType(CW::Renderer::Mesh& mesh, const std::vector<std::uint8_t>& buffer, unsigned int dimension, unsigned int layout, GLenum type) noexcept {
  if(dimension == 0) return;
  if(buffer.empty()) return;

  switch (type){
    case GL_FLOAT:
      uploadTypedBuffer<float>(mesh, buffer, dimension, layout, type);
      break;
    case GL_UNSIGNED_INT:
      uploadTypedBuffer<unsigned int>(mesh, buffer, dimension, layout, type);
      break;
    case GL_UNSIGNED_SHORT:
      uploadTypedBuffer<unsigned short>(mesh, buffer, dimension, layout, type);
      break;
    default:
      break;
  };
};



#ifdef PRODUCTION
inline void scanCmrcDirectoryHelper(const cmrc::embedded_filesystem& fs, const std::filesystem::path& current_path, const std::regex& pattern, std::vector<std::filesystem::path>& out_mesh_files) {
  if(current_path.empty()) return;

  const std::string path_str = current_path.generic_string();
  if (fs.is_file(path_str)){
    if (std::regex_search(current_path.filename().string(), pattern)) {
      out_mesh_files.emplace_back(current_path);
    };
    return;
  };

  if (!fs.is_directory(path_str)) return;
    
  for (const cmrc::directory_entry& entry : fs.iterate_directory(path_str)) {
    std::filesystem::path entry_path = current_path / entry.filename();

    if (entry.is_directory())
      scanCmrcDirectoryHelper(fs, entry_path, pattern, out_mesh_files);
    else if (entry.is_file() && std::regex_search(entry.filename(), pattern))
      out_mesh_files.emplace_back(std::move(entry_path));
  };
};



inline void scanCmrcDirectory(const cmrc::embedded_filesystem& fs, const std::filesystem::path& current_path, const std::string& pattern_str, std::vector<std::filesystem::path>& out_mesh_files) {
  out_mesh_files.clear();
  
  if(pattern_str.empty()) return;
  if(current_path.empty()) return;

  std::regex pattern(pattern_str);
  scanCmrcDirectoryHelper(fs, current_path, pattern, out_mesh_files);
};
#endif
}; // namespace Engine::Utils
