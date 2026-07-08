// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include "Renderer.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <utility>



namespace Engine::Utils {
template<typename T>
class ResourceController {
private:
  std::vector<T> data;
  std::unordered_map<std::string, unsigned int> name_to_id;
  std::vector<std::string> id_to_name;
  unsigned int version = 0;

public:
  ResourceController();
  ~ResourceController();

  T& operator[](unsigned int index);
  
  const T& operator[](unsigned int index) const;

  static constexpr unsigned int INVALID_ID = -1; 
  unsigned int getID(const std::string& name);

  void erase(const std::string& name);
  unsigned int size() const;
  void clear();

  void emplace_back(const std::string& name, T&& mesh);
  bool exists(const std::string& name) const;

  std::unordered_map<std::string, unsigned int>& getIDs();

  bool validateVersion(unsigned int version);
  unsigned int getLatestsVersion();

  void compileAll();

};
};



#include "ResourceController.hpp"
