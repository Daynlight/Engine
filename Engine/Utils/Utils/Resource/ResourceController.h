// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include "CWindow/Renderer/OpenGL/Renderer.h"

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
  ResourceController() noexcept;
  ~ResourceController() noexcept;
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
  void emplace_back(const std::string& name, const T& record) noexcept;
  void emplace_back(const std::string& name, T&& record) noexcept;

  void erase(const std::string& name) noexcept;
  void clear() noexcept;

  T* getResource(unsigned int id) noexcept;

// ================== //
// ==== Data Info === //
// ================== //
public:
  unsigned int getID(const std::string& name) const noexcept;
  bool isIDValid(unsigned int id) const noexcept;
  std::string getName(unsigned int id) const noexcept;
  std::unordered_map<std::string, unsigned int> getNameToID() const noexcept;

  bool exists(const std::string& name) const noexcept;
  unsigned int size() const noexcept;

  bool validateVersion(unsigned int version) const noexcept;
  unsigned int getLatestsVersion() const noexcept;
};
};



#include "ResourceController.hpp"
