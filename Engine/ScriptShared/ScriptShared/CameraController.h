// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#ifdef BUILDING_SCRIPT_DLL
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/quaternion.hpp"
#else
#include "Renderer.h"
#include "../vendor/glm/glm/gtc/quaternion.hpp"
#endif

#include "Camera.h"



namespace Engine::ScriptShared {
class ICameraController {
public:
  virtual Engine::ScriptShared::ICamera& getActiveCamera() = 0;
  virtual Engine::ScriptShared::ICamera& getCamera(const std::string& name) = 0;

  virtual void setActiveCamera(const std::string& name) = 0;
  virtual void spawnCamera(const std::string& name, glm::vec3 position = {0.0f, 0.0f, 0.0f}, glm::vec3 direction = {0.0f, 0.0f, 1.0f}) = 0;
  virtual void deleteCamera(const std::string& name) = 0;

  virtual std::string getActiveCameraName() = 0;
};
};
