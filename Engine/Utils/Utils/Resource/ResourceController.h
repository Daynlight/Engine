// Engine
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
// ========================== //
// ========== Data ========== //
// ========================== //
// ================= //
// ====== Core ===== //
// ================= //
private:
  std::vector<T> data;
  std::unordered_map<std::string, unsigned int> name_to_id;
  std::vector<std::string> id_to_name;
  unsigned int version = 0;



// ========================== //
// ======== Functions ======= //
// ========================== //
// ================== //
// == Constructors == //
// ================== //
public:
// core
  ResourceController();
  ~ResourceController();
// copy
  ResourceController(const ResourceController& second) noexcept;
  ResourceController& operator=(const ResourceController& second) noexcept;
// move
  ResourceController(ResourceController&& second) noexcept;
  ResourceController& operator=(ResourceController&& second) noexcept;

// ================== //
// == Data Control == //
// ================== //
public:
  void emplace_back(const std::string& name, const T& record);
  void emplace_back(const std::string& name, T&& record);

  void erase(const std::string& name);
  void clear();

  T& getResource(unsigned int id);

// ================== //
// ==== Data Info === //
// ================== //
public:
  unsigned int getID(const std::string& name);
  bool isIDValid(unsigned int id);
  std::string getName(unsigned int id);
  std::unordered_map<std::string, unsigned int> getNameToID();

  bool exists(const std::string& name) const;
  unsigned int size() const;

  bool validateVersion(unsigned int version);
  unsigned int getLatestsVersion();

  void compileAll();
};
};



#include "ResourceController.hpp"
