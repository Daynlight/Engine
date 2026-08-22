// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "ResourceController.h"



// ================== //
// == Constructors == //
// ================== //
// core
template<typename T>
Engine::Utils::ResourceController<T>::ResourceController() {};



template<typename T>
Engine::Utils::ResourceController<T>::~ResourceController() {};



// copy
template <typename T>
inline Engine::Utils::ResourceController<T>::ResourceController(const ResourceController &second) noexcept
  : data(second.data),
    name_to_id(second.name_to_id),
    id_to_name(second.id_to_name),
    version(second.version) {};



template <typename T>
inline Engine::Utils::ResourceController<T> &Engine::Utils::ResourceController<T>::operator=(const ResourceController &second) noexcept {
  if(this == &second) return *this;

  data = second.data;
  name_to_id = second.name_to_id;
  id_to_name = second.id_to_name;
  version = second.version;

  return *this;
};



// move
template <typename T>
inline Engine::Utils::ResourceController<T>::ResourceController(ResourceController &&second) noexcept 
  : data(std::move(second.data)),
    name_to_id(std::move(second.name_to_id)),
    id_to_name(std::move(second.id_to_name)),
    version(std::move(second.version)) {};



template <typename T>
inline Engine::Utils::ResourceController<T> &Engine::Utils::ResourceController<T>::operator=(ResourceController &&second) noexcept {
  if(this == &second) return *this;

  data = std::move(second.data);
  name_to_id = std::move(second.name_to_id);
  id_to_name = std::move(second.id_to_name);
  version = std::move(second.version);

  return *this;
};



// ================== //
// == Data Control == //
// ================== //
template<typename T>
void Engine::Utils::ResourceController<T>::emplace_back(const std::string& name, const T& record) {
  version += 1;

  auto it = name_to_id.find(name);
  if (it != name_to_id.end()) {
    data[it->second] = record;
  } else {  
    unsigned int new_id = static_cast<unsigned int>(data.size());
    
    data.emplace_back(record);
    name_to_id[name] = new_id;
    id_to_name.push_back(name);
  };
};



template<typename T>
void Engine::Utils::ResourceController<T>::emplace_back(const std::string& name, T&& record) {
  version += 1;

  auto it = name_to_id.find(name);
  if (it != name_to_id.end()) {
    data[it->second] = std::move(record);
  } else {  
    unsigned int new_id = static_cast<unsigned int>(data.size());
    
    data.emplace_back(std::move(record));
    name_to_id[name] = new_id;
    id_to_name.push_back(name);
  };
};



template<typename T>
void Engine::Utils::ResourceController<T>::erase(const std::string& name) {
  if (!exists(name)) return;
  version += 1;

  unsigned int index_to_remove = name_to_id[name];
  unsigned int last_index = static_cast<unsigned int>(data.size() - 1);

  if (index_to_remove != last_index) {
    std::swap(data[index_to_remove], data[last_index]);

    const std::string& moved_element_name = id_to_name[last_index];

    name_to_id[moved_element_name] = index_to_remove;
    id_to_name[index_to_remove] = moved_element_name;
  };

  data.pop_back();
  id_to_name.pop_back();
  name_to_id.erase(name);
};



template<typename T>
void Engine::Utils::ResourceController<T>::clear(){
  version += 1;
  data.clear();
  name_to_id.clear();
  id_to_name.clear();
};



template <typename T>
inline T &Engine::Utils::ResourceController<T>::getResource(unsigned int id){
  return data[id];
};



// ================== //
// ==== Data Info === //
// ================== //
template<typename T>
unsigned int Engine::Utils::ResourceController<T>::getID(const std::string& name) {
  auto it = name_to_id.find(name);
  if (it == name_to_id.end()) {
    return -1;
  };

  return it->second;
};



template <typename T>
inline std::string Engine::Utils::ResourceController<T>::getName(unsigned int id){
  if(id >= id_to_name.size()) return "";
  return id_to_name[id];
};



template <typename T>
inline std::unordered_map<std::string, unsigned int> Engine::Utils::ResourceController<T>::getNameToID(){
  return name_to_id; 
};



template<typename T>
bool Engine::Utils::ResourceController<T>::exists(const std::string& name) const {
  return name_to_id.find(name) != name_to_id.end();
};



template<typename T>
unsigned int Engine::Utils::ResourceController<T>::size() const{
  return data.size();
};



template<typename T>
bool Engine::Utils::ResourceController<T>::validateVersion(unsigned int version){
  return version == this->version;
};



template<typename T>
unsigned int Engine::Utils::ResourceController<T>::getLatestsVersion(){
  return version;
};



template<typename T>
void Engine::Utils::ResourceController<T>::compileAll(){
  for(T& rec : data) rec.compile();
};
