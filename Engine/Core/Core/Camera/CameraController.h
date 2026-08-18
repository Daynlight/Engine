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
#include "Utils/Logger.h"
#include "Camera.h"



namespace Engine::Core {
class CameraController : public Engine::ScriptShared::ICameraController {
//////// ============================================ ////////
//////// ================== Struct ================== ////////
//////// ============================================ ////////
private:
  CW::Renderer::Renderer* renderer = nullptr;
  std::unordered_map<std::string, Engine::Core::Camera> cameras{};
  std::string active_camera = "";



//////// =============================================== ////////
//////// ================== Functions ================== ////////
//////// =============================================== ////////
//// ====================== ////
//// ==== Constructors ==== ////
//// ====================== ////
public:
//// core
  CameraController() noexcept;
  CameraController(CW::Renderer::Renderer* renderer) noexcept;
  ~CameraController() noexcept;
//// copy
  CameraController(const CameraController& second) noexcept;
  Engine::Core::CameraController& operator=(const CameraController& second) noexcept;
//// move
  CameraController(CameraController&& second) noexcept;
  Engine::Core::CameraController& operator=(CameraController&& second) noexcept;
  
//// ================= ////
//// ==== Control ==== ////
//// ================= ////
  void setActiveCamera(const std::string& name) noexcept;
  std::string getActiveCameraName() const noexcept;
  Engine::ScriptShared::ICamera& getActiveCamera();
  Engine::Core::Camera& getCoreActiveCamera();
  
  void spawnCamera(const std::string& name, glm::vec3 position = {0.0f, 0.0f, 0.0f}, glm::vec3 direction = {0.0f, 0.0f, 1.0f}) noexcept;
  void deleteCamera(const std::string& name) noexcept;

  Engine::ScriptShared::ICamera& getCamera(const std::string& name);
  bool cameraExists(const std::string& name) const noexcept;

};
};
