// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include "Renderer.h"
#include "ResourceController.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <utility>



namespace Engine::Utils {
template<typename T>
class Resource {
// ========================== //
// ========== Data ========== //
// ========================== //
private:
  std::string name = "";
  ResourceController<T>* controller = nullptr;
  unsigned int version = -1;
  unsigned int id = -1;



// ========================== //
// ======== Functions ======= //
// ========================== //
// ================== //
// == Constructors == //
// ================== //
public:
// core
  Resource() noexcept;
  Resource(const std::string& name, ResourceController<T>* controller) noexcept;
  ~Resource() noexcept;

// copy
  Resource(const Resource& other) noexcept;
  Resource& operator=(const Resource& other) noexcept;
// move
  Resource(Resource&& other) noexcept;
  Resource& operator=(Resource&& other) noexcept;

// ================== //
// == Data Control == //
// ================== //
public:  
  T* getResource() noexcept;

  std::string getName() const noexcept;
  void setName(const std::string& name) noexcept;
  bool nameIsValid() const noexcept;

  void setController(ResourceController<T>* controller) noexcept;
  ResourceController<T>* getController() noexcept;

// ================ //
// == Validation == //
// ================ //
private:
  bool validate() noexcept;
};
};



#include "Resource.hpp"
