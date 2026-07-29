// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "Lights.h"



Engine::Core::Light::Light(glm::vec3 position, glm::vec3 color, float strength)
  :position(position), color(color), strength(strength) {};



Engine::Core::Lights::Lights(std::initializer_list<Light> lights)
  : lights(lights) {
  compile();
};



Engine::Core::Lights::~Lights(){
  destroy();
};



void Engine::Core::Lights::compile(){
  buffer.create();
  buffer.set<Light>(lights);
  is_compiled = true;
};



void Engine::Core::Lights::destroy(){
  buffer.destroy();
  is_compiled = false;
};



void Engine::Core::Lights::bind(GLuint socket){
  if(!is_compiled) compile();

  buffer.bind(socket);
};



void Engine::Core::Lights::unbind(){
  if(!is_compiled) return;

  buffer.unbind();
};



Engine::Core::Light& Engine::Core::Lights::operator[](unsigned int index){
  is_compiled = false;
  return lights[index];
};



Engine::Core::Light Engine::Core::Lights::get(unsigned int index) const{
  return lights[index];
};



void Engine::Core::Lights::clear(){
  is_compiled = false;
  lights.clear();
};



void Engine::Core::Lights::erase(unsigned int index){
  is_compiled = false;
  lights.erase(lights.begin() + index);
};



unsigned int Engine::Core::Lights::size() const {
  return lights.size();
};



void Engine::Core::Lights::emplace_back(Light light){
  is_compiled = false;
  lights.emplace_back(light);
};



void Engine::Core::Lights::emplace_back(std::initializer_list<Light> lights){
  is_compiled = false;
  for (Light el : lights) this->lights.emplace_back(el);
};
