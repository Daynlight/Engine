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
private:
  std::string name = "";
  ResourceController<T>* controller = nullptr;
  unsigned int version = -1;
  unsigned int id = -1;
  bool valid = 1;

public:
  Resource() = default;
  Resource(const std::string& name, ResourceController<T>* controller);
  ~Resource();
  Resource(const Resource& other);
  Resource& operator=(const Resource& other);
  Resource(Resource&& other) noexcept;
  Resource& operator=(Resource&& other) noexcept;

  T* get();
  void setName(const std::string& name);

private:
  bool validate();
};
};



#include "Resource.hpp"
