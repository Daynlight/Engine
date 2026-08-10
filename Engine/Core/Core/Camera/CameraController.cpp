// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "CameraController.h"



Engine::Core::CameraController::CameraController(CW::Renderer::Renderer *renderer)
  :renderer(renderer){};


Engine::ScriptShared::ICamera &Engine::Core::CameraController::getActiveCamera() {
  // [TODO] No Camera
  return cameras[active_camera];
};


Engine::ScriptShared::ICamera &Engine::Core::CameraController::getCamera(const std::string &name){
  // [TODO] No Camera
  return cameras[name];
};


void Engine::Core::CameraController::setActiveCamera(const std::string &name){
  active_camera = name;
};


void Engine::Core::CameraController::spawnCamera(const std::string &name, glm::vec3 position, glm::vec3 direction){
  cameras[name] = std::move(Engine::Core::Camera(renderer, position, direction));
};


void Engine::Core::CameraController::deleteCamera(const std::string &name){
  cameras.erase(name);
};


std::string Engine::Core::CameraController::getActiveCameraName(){
  return active_camera;
};