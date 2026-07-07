// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "ResourceController.h"



template<typename T>
UW::ResourceController<T>::ResourceController() {
};



template<typename T>
UW::ResourceController<T>::~ResourceController() {
};



template<typename T>
T& UW::ResourceController<T>::operator[](unsigned int index) {
  return data[index];
};



template<typename T>
const T& UW::ResourceController<T>::operator[](unsigned int index) const {
  return data[index];
};



template<typename T>
unsigned int UW::ResourceController<T>::getID(const std::string& name) {
  auto it = name_to_id.find(name);
  if (it == name_to_id.end()) {
    return INVALID_ID;
  };
  return it->second;
};




template<typename T>
bool UW::ResourceController<T>::exists(const std::string& name) const {
  return name_to_id.find(name) != name_to_id.end();
};



template<typename T>
void UW::ResourceController<T>::emplace_back(const std::string& name, T&& mesh) {
  version += 1;

  auto it = name_to_id.find(name);
  if (it != name_to_id.end()) {
    data[it->second] = std::move(mesh);
  } else {  
    unsigned int new_id = static_cast<unsigned int>(data.size());
    
    data.emplace_back(std::move(mesh));
    name_to_id[name] = new_id;
    id_to_name.push_back(name);
  };
};




template<typename T>
void UW::ResourceController<T>::erase(const std::string& name) {
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
unsigned int UW::ResourceController<T>::size() const{
  return data.size();
};




template<typename T>
void UW::ResourceController<T>::clear(){
  version += 1;
  data.clear();
  name_to_id.clear();
  id_to_name.clear();
};



template<typename T>
std::unordered_map<std::string, unsigned int>& UW::ResourceController<T>::getIDs(){
  return name_to_id;
};



template<typename T>
bool UW::ResourceController<T>::validateVersion(unsigned int version){
  return version == this->version;
};



template<typename T>
unsigned int UW::ResourceController<T>::getLatestsVersion(){
  return version;
};



template<typename T>
void UW::ResourceController<T>::compileAll(){
  for(T& rec : data) rec.compile();
};



template<typename T>
void UW::ResourceController<T>::manualVersionUpdate(){
  version++;
};