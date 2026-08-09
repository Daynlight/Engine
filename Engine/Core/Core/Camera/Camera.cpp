// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "Camera.h"



Engine::Camera::Camera(){
  resetMouse();
};



Engine::Camera::Camera(CW::Renderer::Renderer* renderer, glm::vec3 position, glm::vec3 direction) 
  : renderer(renderer), position(position) {
  if (glm::length(direction) > 0.0001f) {
    this->direction = glm::normalize(direction);
    this->orientation = glm::quatLookAt(-this->direction, glm::vec3(0.0f, 1.0f, 0.0f));
  } else {
    this->direction = glm::vec3(0.0f, 0.0f, 1.0f);
    this->orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  };

  resetMouse();
};



Engine::Camera& Engine::Camera::operator=(Camera &&second){
  if (this != &second) {
    renderer = second.renderer; 
    second.renderer = nullptr;
    
    position = std::move(second.position);
    is_ortho = second.is_ortho;
    
    if (glm::length(second.direction) > 0.0001f) {
      this->direction = glm::normalize(second.direction);
      this->orientation = glm::quatLookAt(-this->direction, glm::vec3(0.0f, 1.0f, 0.0f));
    } else {
      this->direction = glm::vec3(0.0f, 0.0f, 1.0f);
      this->orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }
  }

  resetMouse();
  return *this;
};



void Engine::Camera::rotate(float xoffset, float yoffset, float zoffset) {
  glm::quat qPitch = glm::angleAxis(glm::radians(yoffset * sensitivity), glm::vec3(1.0f, 0.0f, 0.0f));
  glm::quat qYaw   = glm::angleAxis(glm::radians(-xoffset * sensitivity), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::quat qRoll  = glm::angleAxis(glm::radians(zoffset), glm::vec3(0.0f, 0.0f, 1.0f));
  
  orientation = orientation * qPitch * qYaw * qRoll;
  orientation = glm::normalize(orientation);
  updateDirection();
};



void Engine::Camera::updateDirection() {
  direction = orientation * glm::vec3(0.0f, 0.0f, 1.0f);
  direction = glm::normalize(direction);
};



glm::mat4 Engine::Camera::transformation(){
  return projection() * view();
};



glm::mat4 Engine::Camera::view(){
  glm::vec3 dynamicUp = orientation * glm::vec3(0.0f, 1.0f, 0.0f);
  return glm::lookAt(position, position + direction, dynamicUp);
};



glm::mat4 Engine::Camera::projection() {
  if (!renderer || !renderer->getWindowData()) 
    return glm::mat4(1.0f); 

  float aspectRatio = renderer->getWindowData()->width / (float)renderer->getWindowData()->height;

  if (is_ortho) {
    float orthoSize = fov; 
    float halfWidth = (orthoSize * aspectRatio) * 0.5f;
    float halfHeight = orthoSize * 0.5f;

    return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, Engine::Config::CAMERA_NEAR_PLANE, Engine::Config::CAMERA_ORTHO_FAR_PLANE);
  } else {
    return glm::perspective(glm::radians(Engine::Config::CAMERA_FOV), aspectRatio, Engine::Config::CAMERA_NEAR_PLANE, Engine::Config::CAMERA_FAR_PLANE);
  };
};



void Engine::Camera::event() {
  if(!default_movemement_on) return;

  float dt = renderer->getWindowData()->delta_time;

  if (cursor_lock) renderer->setCursorOn(true);
  else renderer->setCursorOn(false);

  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_SWAP_MODE_BTN) && cursor_visible_lock <= 0.0f) {
    cursor_lock = !cursor_lock;
    cursor_visible_lock = Engine::Config::CAMERA_SWAP_COOLDOWN;
    resetMouse();
  }
  else if (cursor_visible_lock > 0.0f) {
    cursor_visible_lock -= dt;
  };

  if (cursor_lock) return;

  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_ACCELERATE)) velocity += Engine::Config::CAMERA_ACCELERATION_RATE * dt;
  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_DECELERATE)) velocity -= Engine::Config::CAMERA_ACCELERATION_RATE * dt;
  if (velocity < Engine::Config::CAMERA_MIN_VELOCITY) velocity = Engine::Config::CAMERA_MIN_VELOCITY;

  glm::vec3 right = orientation * glm::vec3(1.0f, 0.0f, 0.0f);
  float target_bank = 0.0f;

  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_MOVE_FORWARD)) position += direction * velocity * dt;
  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_MOVE_BACK))    position -= direction * velocity * dt;
  
  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_MOVE_RIGHT)) {
    position -= right * velocity * dt;
    target_bank -= Engine::Config::CAMERA_TILT_ACCELERATION;
  };
  
  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_MOVE_LEFT)) {
    position += right * velocity * dt;
    target_bank += Engine::Config::CAMERA_TILT_ACCELERATION;
  };

  float xoffset = renderer->getInputData()->mouse_x - lastMouseX;
  float yoffset = renderer->getInputData()->mouse_y - lastMouseY; 
  lastMouseX = renderer->getInputData()->mouse_x;
  lastMouseY = renderer->getInputData()->mouse_y;

  glm::vec3 localUp = glm::inverse(orientation) * glm::vec3(0.0f, 1.0f, 0.0f);
  float current_roll = glm::degrees(glm::atan2(localUp.x, localUp.y));
  
  float zoffset = 0.0f;
  bool manual_roll = false;

  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_ROLL_LEFT)) {
    if (current_roll > -Engine::Config::CAMERA_MAX_TILT){
      zoffset -= Engine::Config::CAMERA_MANUAL_ROLL_SPEED * dt;
      manual_roll = true;
    };
  };
  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_ROLL_RIGHT)) {
    if (current_roll < Engine::Config::CAMERA_MAX_TILT){
      zoffset += Engine::Config::CAMERA_MANUAL_ROLL_SPEED * dt;
      manual_roll = true;
    };
  };

  if (!manual_roll) zoffset += (target_bank - current_roll) * Engine::Config::CAMERA_ROLL_INTERPOLATION_SPEED * dt;

  rotate(mouse_is_active ? xoffset : 0.0f, mouse_is_active ? yoffset : 0.0f, zoffset);
  mouse_is_active = true;
};



void Engine::Camera::setOrthographic(bool enable){
  is_ortho = enable;
};



void Engine::Camera::resetMouse(){
  mouse_is_active = false;
};


glm::vec3 Engine::Camera::getPosition(){
  return position;
};


void Engine::Camera::setPosition(glm::vec3 position){
  this->position = position;
};


glm::vec3 Engine::Camera::getDirection(){
  return direction;
};


void Engine::Camera::setDirection(glm::vec3 direction){
  if (glm::length(direction) > 0.0001f) {
    this->direction = glm::normalize(direction);
    this->orientation = glm::quatLookAt(-this->direction, glm::vec3(0.0f, 1.0f, 0.0f));
  } else {
    this->direction = glm::vec3(0.0f, 0.0f, 1.0f);
    this->orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  };
  
  resetMouse();
};



void Engine::Camera::setFov(float fov){
  this->fov = fov;
}