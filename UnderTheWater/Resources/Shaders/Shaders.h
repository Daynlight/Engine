#pragma once
#include "Renderer.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <utility>



namespace UW {
class Shaders {
private:
  std::vector<CW::Renderer::Shader> shader_data;
  std::unordered_map<std::string, unsigned int> shader_id;
  std::vector<std::string> id_to_name;

  unsigned int version = 0;

public:
  Shaders();
  ~Shaders();

  CW::Renderer::Shader& operator[](unsigned int index);
  
  const CW::Renderer::Shader& operator[](unsigned int index) const;

  static constexpr unsigned int INVALID_ID = -1; 
  unsigned int get_id(const std::string& name);

  void erase(const std::string& name);
  void clear();

  void emplace_back(const std::string& name, CW::Renderer::Shader&& mesh);
  bool exists(const std::string& name) const;

  std::unordered_map<std::string, unsigned int>& getShadersIDs();

  bool validateVersion(unsigned int version);
  unsigned int getLatestsVersion();
};
};