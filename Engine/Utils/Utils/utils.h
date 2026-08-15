// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include <filesystem>
#include <string>
#include <regex>

#ifdef PRODUCTION
#include <cmrc/cmrc.hpp>
#endif



namespace Engine::Utils {
constexpr inline uint32_t hash(uint32_t x) noexcept {
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = (x >> 16) ^ x;
  return x;
};



template<typename T>
inline void uploadTypedBuffer(CW::Renderer::Mesh& mesh, const std::vector<std::uint8_t>& buffer, unsigned int dimension, unsigned int layout, GLenum type) noexcept {
  if (buffer.empty()) return;
  if (buffer.size() % sizeof(T) != 0) return;

  size_t count = buffer.size() / sizeof(T);

  std::vector<T> typed;
  typed.resize(count);

  std::memcpy(typed.data(), buffer.data(), buffer.size());
  mesh.setData<T>(std::move(typed), dimension, layout, type);
};



inline void uploadBufferByType(CW::Renderer::Mesh& engine_mesh, GLenum type, const std::vector<std::uint8_t>& buffer, unsigned int dimension, unsigned int key) noexcept {
  if (buffer.empty()) return;

  switch (type){
    case GL_FLOAT:
      uploadTypedBuffer<float>(engine_mesh, buffer, dimension, key, type);
      break;
    case GL_UNSIGNED_INT:
      uploadTypedBuffer<unsigned int>(engine_mesh, buffer, dimension, key, type);
      break;
    case GL_UNSIGNED_SHORT:
      uploadTypedBuffer<unsigned short>(engine_mesh, buffer, dimension, key, type);
      break;
    default:
      break;
  };
};



#ifdef PRODUCTION
inline void scanCmrcDirectory(const cmrc::embedded_filesystem& fs, const std::string& current_path, const std::string& pattern_str, std::vector<std::string>& out_mesh_files) noexcept {
  std::regex pattern(pattern_str);

  for (const auto& entry : fs.iterate_directory(current_path)) {
    std::string entry_path = current_path + (current_path.back() == '/' ? "" : "/") + entry.filename();
  
    if (entry.is_directory()) scanCmrcDirectory(fs, entry_path, pattern_str, out_mesh_files);
    else if (entry.is_file() && std::regex_search(entry.filename(), pattern)) out_mesh_files.emplace_back(entry_path);
  };
};
#endif
}; // namespace Engine::Utils
