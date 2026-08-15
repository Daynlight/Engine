// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "Materials.h"



Engine::Core::Material::Material(
  glm::vec3 albedo, 
  float metallic,
  float roughness,
  glm::vec3 emission_color,
  float emission_strength,
  float ambient_occlusion):
  albedo(albedo),
  metallic(metallic),
  roughness(roughness),
  emission_color(emission_color),
  emission_strength(emission_strength),
  ambient_occlusion(ambient_occlusion){};



Engine::Core::Materials::~Materials(){
  destroy();
};



void Engine::Core::Materials::genVectors(){
  materials.clear();
  material_translate.clear();

  for(const std::pair<std::string, Material>& el : material_reg){
    material_translate[el.first] = materials.size();
    materials.emplace_back(el.second);
  };
};



void Engine::Core::Materials::compile(){
  buffer.create();
  genVectors();
  buffer.set<Material>(materials);
  is_compiled = true;
};



void Engine::Core::Materials::destroy(){
  buffer.destroy();
  is_compiled = false;
};



void Engine::Core::Materials::bind(GLuint socket){
  if(!is_compiled) compile();

  buffer.bind(socket);
};



void Engine::Core::Materials::unbind(){
  if(!is_compiled) return;

  buffer.unbind();
};



unsigned int Engine::Core::Materials::translate_material(const std::string& name){
  return material_translate[name];
};



Engine::Core::Material& Engine::Core::Materials::operator[](const std::string& name){
  is_compiled = false;
  return material_reg[name];
};



const std::unordered_map<std::string, Engine::Core::Material>& Engine::Core::Materials::getMaterialReg(){
  return material_reg;
};



bool Engine::Core::Materials::find(const std::string& name){
  auto it = material_reg.find(name);
  if(it == material_reg.end()) return false;
  return true;
};



Engine::Core::Material Engine::Core::Materials::getMaterial(const std::string& name){
  return material_reg[name];
};



void Engine::Core::Materials::clear(){
  is_compiled = false;
  material_reg.clear();
  materials.clear();
};



void Engine::Core::Materials::erase(const std::string& name){
  is_compiled = false;
  material_reg.erase(name);
};



unsigned int Engine::Core::Materials::size() const {
  return material_reg.size();
};



void Engine::Core::Materials::emplace_back(const std::string& name, Material material){
  is_compiled = false;
  material_reg[name] = material;
};



void Engine::Core::Materials::emplace_back(std::initializer_list<std::pair<std::string, Material>> materials){
  is_compiled = false;
  for (std::pair<std::string, Material> el : materials) this->material_reg[el.first] = el.second;
};
