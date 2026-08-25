// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "Resource.h"



// ================== //
// == Constructors == //
// ================== //
// core
template<typename T>
Engine::Utils::Resource<T>::Resource() noexcept {};



template<typename T>
Engine::Utils::Resource<T>::Resource(const std::string& name, ResourceController<T>* controller) noexcept
  :name(name), controller(controller) {
  if(!this->controller) version = -1; 
  else version = this->controller->getLatestsVersion() - 1;
};



template<typename T>
Engine::Utils::Resource<T>::~Resource() noexcept {
};



// copy
template<typename T>
Engine::Utils::Resource<T>::Resource(const Resource& other) noexcept
  :name(other.name), 
   controller(other.controller), 
   version(other.version), 
   id(other.id) {};



template <typename T>
Engine::Utils::Resource<T>& Engine::Utils::Resource<T>::operator=(const Resource &other) noexcept {
  if(this == &other) return *this;

  name = other.name;
  controller = other.controller;
  version = other.version;
  id = other.id;

  return *this;
};



// move
template <typename T>
Engine::Utils::Resource<T>::Resource(Resource &&other) noexcept
  : name(std::move(other.name)), 
    controller(std::move(other.controller)), 
    version(std::move(other.version)), 
    id(std::move(other.id)) {
  other.controller = nullptr;
};




template <typename T>
Engine::Utils::Resource<T>& Engine::Utils::Resource<T>::operator=(Resource &&other) noexcept {
  if(this == &other) return *this;

  name = std::move(other.name);
  controller = std::move(other.controller);
  version = std::move(other.version);
  id = std::move(other.id);

  other.controller = nullptr;
  
  return *this;
};



// ================== //
// == Data Control == //
// ================== //
template<typename T>
T* Engine::Utils::Resource<T>::getResource() noexcept {
  if(!controller) return nullptr;

  bool valid = validate();
  if(!valid) return nullptr;
  
  return controller->getResource(id);
};



template <typename T>
inline std::string Engine::Utils::Resource<T>::getName() const noexcept {
  return name;
};



template<typename T>
inline void Engine::Utils::Resource<T>::setName(const std::string& name) noexcept {
  this->name = name;
  if(!controller) version = -1; 
  else version = controller->getLatestsVersion() - 1;
};



template <typename T>
inline bool Engine::Utils::Resource<T>::nameIsValid() const noexcept {
  if(!controller) return false;
  return controller->exists(name);
};



template <typename T>
inline void Engine::Utils::Resource<T>::setController(Engine::Utils::ResourceController<T> *controller) noexcept {
  this->controller = controller;
};



template <typename T>
inline Engine::Utils::ResourceController<T> *Engine::Utils::Resource<T>::getController() noexcept {
  return controller;
};



// ================ //
// == Validation == //
// ================ //
template<typename T>
bool Engine::Utils::Resource<T>::validate() noexcept {
  if(!controller) return 0;

  if(!controller->validateVersion(version)){
    if(!controller->exists(name)) return 0;
    id = controller->getID(name);
    version = controller->getLatestsVersion();
  };

  if(id >= controller->size()) return 0;

  return 1;
};
