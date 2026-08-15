// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "Resource.h"



template<typename T>
Engine::Utils::Resource<T>::Resource(const std::string& name, ResourceController<T>* controller) 
  :name(name), controller(controller) {
  valid = validate();
};



template<typename T>
Engine::Utils::Resource<T>::~Resource() {
};



template<typename T>
Engine::Utils::Resource<T>::Resource(const Resource& other)
  :name(other.name), controller(other.controller), version(other.version), id(other.id), valid(other.valid){
};



template <typename T>
Engine::Utils::Resource<T>& Engine::Utils::Resource<T>::operator=(const Resource &other){
  name = other.name;
  controller = other.controller;
  version = other.version;
  id = other.id;
  valid = other.valid;

  return *this;
};



template <typename T>
Engine::Utils::Resource<T>::Resource(Resource &&other) noexcept
  : name(std::move(other.name)), controller(std::move(other.controller)), version(std::move(other.version)), id(std::move(other.id)), valid(std::move(other.valid)){
};



template <typename T>
Engine::Utils::Resource<T>& Engine::Utils::Resource<T>::operator=(Resource &&other) noexcept{
  name = std::move(other.name);
  controller = std::move(other.controller);
  version = std::move(other.version);
  id = std::move(other.id);
  valid = std::move(other.valid);

  return *this;
};



template<typename T>
T* Engine::Utils::Resource<T>::get(){
  // if(!valid) return nullptr;

  valid = validate();
  if(!valid) return nullptr;
  
  return &((*controller)[id]);
};



template<typename T>
void Engine::Utils::Resource<T>::setName(const std::string& name){
  this->name = name;
  version = controller->getLatestsVersion() - 1;
};



template<typename T>
bool Engine::Utils::Resource<T>::validate(){
  if(!controller) return 0;

  if(!controller->exists(name)) return 0;

  if(!controller->validateVersion(version)){
    id = controller->getID(name);
    version = controller->getLatestsVersion();
  };

  if(id >= controller->size()) return 0;

  return 1;
};