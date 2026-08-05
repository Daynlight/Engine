// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include "Renderer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "../vendor/glm/glm/gtx/euler_angles.hpp"
#include "../vendor/glm/glm/gtx/quaternion.hpp"

#include "ScriptShared/Camera.h"

#include "Utils/config.h"



namespace Engine {
class Camera : public ICamera {
public:
  glm::vec3 position = {0.0f, 0.0f, 0.0f};
  glm::vec3 direction = {0.0f, 0.0f, 1.0f};
  float fov = Engine::Config::CAMERA_FOV;
  bool default_movemement_on = false;
  CW::Renderer::Renderer* renderer;
  
private:
  glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); 
  bool is_ortho = false;

  float sensitivity = Engine::Config::CAMERA_SENSITIVITY;
  float velocity = Engine::Config::CAMERA_DEFAULT_VELOCITY;
  
  float lastMouseX = 0.0f;
  float lastMouseY = 0.0f;
  bool mouse_is_active = false;

  float cursor_visible_lock = 0.0f;
  bool cursor_lock = true;

public:
  Camera();
  Camera(CW::Renderer::Renderer* renderer, glm::vec3 position = {0.0f, 0.0f, 0.0f}, glm::vec3 direction = {0.0f, 0.0f, 1.0f});
  Engine::Camera& operator=(Camera&& second);

  void rotate(float xoffset, float yoffset, float zoffset);
  void updateDirection();

  glm::mat4 transformation();
  glm::mat4 projection();
  glm::mat4 view();
  
  glm::vec3 getPosition();
  void setPosition(glm::vec3 position);
  glm::vec3 getDirection();
  void setDirection(glm::vec3 direction);
  
  void resetMouse();
  void event();
  void setOrthographic(bool enable);

};
};
