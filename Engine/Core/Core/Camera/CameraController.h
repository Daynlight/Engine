// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include "Renderer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "../vendor/glm/glm/gtx/euler_angles.hpp"
#include "../vendor/glm/glm/gtx/quaternion.hpp"

#include "ScriptShared/CameraController.h"

#include "Utils/config.h"
#include "Camera.h"



namespace Engine::Core {
class CameraController : public Engine::ScriptShared::ICameraController{
private:
  std::unordered_map<std::string, Engine::Core::Camera> cameras;
  CW::Renderer::Renderer* renderer;
  std::string active_camera = "";

public:
  CameraController(CW::Renderer::Renderer* renderer);
  
  Engine::ScriptShared::ICamera& getActiveCamera();
  Engine::ScriptShared::ICamera& getCamera(const std::string& name);

  void setActiveCamera(const std::string& name);
  void spawnCamera(const std::string& name, glm::vec3 position = {0.0f, 0.0f, 0.0f}, glm::vec3 direction = {0.0f, 0.0f, 1.0f});
  void deleteCamera(const std::string& name);

  std::string getActiveCameraName();
};
};
