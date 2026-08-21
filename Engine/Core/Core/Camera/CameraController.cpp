// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "CameraController.h"



//// ====================== ////
//// ==== Constructors ==== ////
//// ====================== ////
//// core
Engine::Core::CameraController::CameraController() noexcept {
  Engine::Utils::Logger::get().warn("Engine::Core::CameraController::CameraController()", "renderer is nullptr");
};



Engine::Core::CameraController::CameraController(CW::Renderer::Renderer *renderer) noexcept
  :renderer(renderer){};



Engine::Core::CameraController::~CameraController() noexcept {};



//// copy
Engine::Core::CameraController::CameraController(const CameraController &second) noexcept
  :renderer(second.renderer),
   cameras(second.cameras),
   active_camera(second.active_camera)
{
  if(renderer == nullptr){
    Engine::Utils::Logger::get().warn("Engine::Core::CameraController::CameraController(const CameraController &second)", "renderer is nullptr");
  };

  active_camera_setted = cameraExists(active_camera);
  if(active_camera_setted) active_camera_ref = &getCoreCamera(active_camera);
};



Engine::Core::CameraController &Engine::Core::CameraController::operator=(const CameraController &second) noexcept {
  if(second.renderer == nullptr){
    Engine::Utils::Logger::get().warn("Engine::Core::CameraController::operator=(const CameraController &second)", "renderer is nullptr");
  };

  if (this == &second) return *this;

  renderer = second.renderer;
  cameras = second.cameras;
  active_camera = second.active_camera;
  active_camera_setted = cameraExists(active_camera);
  if(active_camera_setted) active_camera_ref = &getCoreCamera(active_camera);

  return *this;
};



//// move
Engine::Core::CameraController::CameraController(CameraController &&second) noexcept
  :renderer(std::move(second.renderer)),
   cameras(std::move(second.cameras)),
   active_camera(std::move(second.active_camera))
{
  if(renderer == nullptr){
    Engine::Utils::Logger::get().warn("Engine::Core::CameraController::CameraController(CameraController &&second)", "renderer is nullptr");
  };

  active_camera_setted = cameraExists(active_camera);
  if(active_camera_setted) active_camera_ref = &getCoreCamera(active_camera);

  second.renderer = nullptr;
  second.active_camera_ref = nullptr;
  second.active_camera_setted = false;

};



Engine::Core::CameraController &Engine::Core::CameraController::operator=(CameraController &&second) noexcept{
  if(second.renderer == nullptr){
    Engine::Utils::Logger::get().warn("Engine::Core::CameraController::operator=(CameraController &&second)", "renderer is nullptr");
  };

  if (this == &second) return *this;

  renderer = std::move(second.renderer);
  cameras = std::move(second.cameras);
  active_camera = std::move(second.active_camera);
  active_camera_setted = cameraExists(active_camera);
  if(active_camera_setted) active_camera_ref = &getCoreCamera(active_camera);

  second.renderer = nullptr;
  second.active_camera_ref = nullptr;
  second.active_camera_setted = false;

  return *this;
};



//// ================= ////
//// ==== Control ==== ////
//// ================= ////
void Engine::Core::CameraController::setActiveCamera(const std::string &name) noexcept {
  if(!cameraExists(name)){
    Engine::Utils::Logger::get().warn("Engine::Core::CameraController::setActiveCamera(const std::string &name)", "Camera: " + name + " didn't exists (skipping)");
    return;
  };

  active_camera = name;
  active_camera_setted = cameraExists(active_camera);
  active_camera_ref = &getCoreCamera(active_camera);
};



std::string Engine::Core::CameraController::getActiveCameraName() const noexcept {
  if(!cameraExists(active_camera)){
    Engine::Utils::Logger::get().warn("Engine::Core::CameraController::getActiveCameraName()", "Camera: " + active_camera + " didn't exists (returning '')");
    return "";
  };

  return active_camera;
};



Engine::ScriptShared::ICamera &Engine::Core::CameraController::getActiveCamera() {
  if(!active_camera_setted){
    Engine::Utils::Logger::get().erro("Engine::Core::CameraController::getActiveCamera()", "Camera: " + active_camera + " didn't exists (returning nullptr)");
    throw std::runtime_error("Engine::Core::CameraController::getActiveCamera(const std::string &name) -> Camera: " + active_camera + " didn't exists");
  };

  return *active_camera_ref;
};



Engine::Core::Camera &Engine::Core::CameraController::getCoreActiveCamera() {
  if(!active_camera_setted){
    Engine::Utils::Logger::get().erro("Engine::Core::CameraController::getActiveCamera()", "Camera: " + active_camera + " didn't exists (returning nullptr)");
    throw std::runtime_error("Engine::Core::CameraController::getCoreActiveCamera(const std::string &name) -> Camera: " + active_camera + " didn't exists");
  };

  return *active_camera_ref;
};



void Engine::Core::CameraController::spawnCamera(const std::string &name, glm::vec3 position, glm::vec3 direction) noexcept {
  if(cameraExists(name)){
    Engine::Utils::Logger::get().erro("Engine::Core::CameraController::spawnCamera(const std::string &name, glm::vec3 position, glm::vec3 direction)", "Camera: " + name + " exists (skipping)");
    return;
  };

  cameras.try_emplace(name, renderer, position, direction);
  
  Engine::Utils::Logger::get().info("Engine::Core::CameraController::spawnCamera(const std::string &name, glm::vec3 position, glm::vec3 direction)", "Spawned Camera: " + name);

  active_camera_setted = cameraExists(active_camera);
  if(active_camera_setted) active_camera_ref = &getCoreCamera(active_camera);
};



void Engine::Core::CameraController::deleteCamera(const std::string &name) noexcept {
  if(!cameraExists(name)){
    Engine::Utils::Logger::get().erro("Engine::Core::CameraController::deleteCamera(const std::string &name)", "Camera: " + name + " didn't exists (skipping)");
    return;
  };

  cameras.erase(name);

  Engine::Utils::Logger::get().info("Engine::Core::CameraController::deleteCamera(const std::string &name)", "Deleted Camera: " + name);

  if(active_camera == name){
    active_camera.clear();
    Engine::Utils::Logger::get().info("Engine::Core::CameraController::deleteCamera(const std::string &name)", "Active Camera Reset");
  };

  active_camera_setted = cameraExists(active_camera);
  if(active_camera_setted) active_camera_ref = &getCoreCamera(active_camera);
};



Engine::ScriptShared::ICamera &Engine::Core::CameraController::getCamera(const std::string &name) noexcept {
  return cameras[name];
};



Engine::Core::Camera &Engine::Core::CameraController::getCoreCamera(const std::string &name) noexcept {
  return cameras[name];
};



bool Engine::Core::CameraController::cameraExists(const std::string &name) const noexcept {
  const auto& it = cameras.find(name);
  if(it != cameras.end()) return true;
  return false;
};
