// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "CameraController.h"



Engine::CameraController::CameraController(CW::Renderer::Renderer *renderer)
  :renderer(renderer){};


Engine::ICamera &Engine::CameraController::getActiveCamera() {
  // [TODO] No Camera
  return cameras[active_camera];
};


Engine::ICamera &Engine::CameraController::getCamera(const std::string &name){
  // [TODO] No Camera
  return cameras[name];
};


void Engine::CameraController::setActiveCamera(const std::string &name){
  active_camera = name;
};


void Engine::CameraController::spawnCamera(const std::string &name, glm::vec3 position, glm::vec3 direction){
  cameras[name] = std::move(Engine::Camera(renderer, position, direction));
};


void Engine::CameraController::deleteCamera(const std::string &name){
  cameras.erase(name);
};


std::string Engine::CameraController::getActiveCameraName(){
  return active_camera;
};