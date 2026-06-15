#include "Shaders.h"



UW::Shaders::Shaders() {}



UW::Shaders::~Shaders() {}



CW::Renderer::Shader& UW::Shaders::operator[](unsigned int index) {
  return shader_data[index];
};



const CW::Renderer::Shader& UW::Shaders::operator[](unsigned int index) const {
  return shader_data[index];
};



unsigned int UW::Shaders::get_id(const std::string& name) {
  auto it = shader_id.find(name);
  if (it == shader_id.end()) {
    return INVALID_ID;
  }
  return it->second;
};



bool UW::Shaders::exists(const std::string& name) const {
  return shader_id.find(name) != shader_id.end();
};



void UW::Shaders::emplace_back(const std::string& name, CW::Renderer::Shader&& mesh) {
  version += 1;

  auto it = shader_id.find(name);
  if (it != shader_id.end()) {
    shader_data[it->second] = std::move(mesh);
  } else {  
    unsigned int new_id = static_cast<unsigned int>(shader_data.size());
    
    shader_data.emplace_back(std::move(mesh));
    shader_id[name] = new_id;
    id_to_name.push_back(name);
  };
};



void UW::Shaders::erase(const std::string& name) {
  if (!exists(name)) return;
  version += 1;

  unsigned int index_to_remove = shader_id[name];
  unsigned int last_index = static_cast<unsigned int>(shader_data.size() - 1);

  if (index_to_remove != last_index) {
    std::swap(shader_data[index_to_remove], shader_data[last_index]);

    std::string moved_element_name = id_to_name[last_index];

    shader_id[moved_element_name] = index_to_remove;
    id_to_name[index_to_remove] = std::move(moved_element_name);
  }

  shader_data.pop_back();
  id_to_name.pop_back();
  shader_id.erase(name);
};



void UW::Shaders::clear() {
  version += 1;
  shader_data.clear();
  shader_id.clear();
  id_to_name.clear();
};



std::unordered_map<std::string, unsigned int>& UW::Shaders::getShadersIDs() {
  return shader_id;
};



bool UW::Shaders::validateVersion(unsigned int version) {
  return version == this->version;
};



unsigned int UW::Shaders::getLatestsVersion() {
  return version;
};
