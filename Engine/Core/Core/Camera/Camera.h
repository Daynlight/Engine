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
#include "Utils/Logger.h"



namespace Engine::Core {
class Camera : public Engine::ScriptShared::ICamera {
//////// ============================================ ////////
//////// ================== Struct ================== ////////
//////// ============================================ ////////
//// ============== ////
//// ==== Core ==== ////
//// ============== ////
private:
  CW::Renderer::Renderer* renderer = nullptr;
  glm::vec3 position = {0.0f, 0.0f, 0.0f};
  glm::vec3 direction = {0.0f, 0.0f, 1.0f};
  glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); 

//// ==================== ////
//// ==== Projection ==== ////
//// ==================== ////
private:  
  Engine::ScriptShared::CameraMode mode = Engine::ScriptShared::CameraMode::PERSPECTIVE;
  
  bool transform_mat_ready = false;
  glm::mat4 transform_mat = glm::mat4(1.0f);
  
  bool view_mat_ready = false;
  glm::mat4 view_mat = glm::mat4(1.0f);

  float last_aspect_ratio_perspective = -1.0f;
  float fov = Engine::Config::CAMERA_FOV;
  float perspective_near_plane = Engine::Config::CAMERA_NEAR_PLANE;
  float perspective_far_plane = Engine::Config::CAMERA_FAR_PLANE;
  bool perspective_mat_ready = false;
  glm::mat4 perspective_mat = glm::mat4(1.0f);

  float last_aspect_ratio_orthogonal = -1.0f;
  float ortho_size = Engine::Config::CAMERA_ORTHO_SIZE;
  float orthogonal_near_plane = Engine::Config::CAMERA_ORTHO_NEAR_PLANE;
  float orthogonal_far_plane = Engine::Config::CAMERA_ORTHO_FAR_PLANE;
  bool orthogonal_mat_ready = false;
  glm::mat4 orthogonal_mat = glm::mat4(1.0f);
  
//// ================== ////
//// ==== Movement ==== ////
//// ================== ////
private:  
  bool default_movemement_on = false;

  float sensitivity = Engine::Config::CAMERA_SENSITIVITY;
  float velocity = Engine::Config::CAMERA_DEFAULT_VELOCITY;
  
  float lastMouseX = 0.0f;
  float lastMouseY = 0.0f;
  bool mouse_is_active = false;

  float cursor_visible_lock = 0.0f;
  bool cursor_lock = true;



//////// =============================================== ////////
//////// ================== Functions ================== ////////
//////// =============================================== ////////
//// ====================== ////
//// ==== Constructors ==== ////
//// ====================== ////
public:
//// core
  Camera() noexcept;
  ~Camera() noexcept;
  Camera(CW::Renderer::Renderer* renderer, glm::vec3 position = {0.0f, 0.0f, 0.0f}, glm::vec3 direction = {0.0f, 0.0f, 1.0f}) noexcept;
//// copy
  Camera(const Camera& second) noexcept;
  Engine::Core::Camera& operator=(const Camera& second) noexcept;
//// move
  Camera(Camera&& second) noexcept;
  Engine::Core::Camera& operator=(Camera&& second) noexcept;

//// ==================== ////
//// ==== Projection ==== ////
//// ==================== ////
public:
  glm::mat4 transformation() noexcept;
  glm::mat4 view() noexcept;
  glm::mat4 projection() noexcept;
  
private:
  glm::mat4 perspective_projection() noexcept;
  glm::mat4 orthogonal_projection() noexcept;

//// ========================= ////
//// ==== Setters/Getters ==== ////
//// ========================= ////
public:
  glm::vec3 getPosition() const noexcept;
  void setPosition(glm::vec3 position) noexcept;
  glm::vec3 getDirection() const noexcept;
  void setDirection(glm::vec3 direction) noexcept;
  
  float getFov() const noexcept;
  void setFov(float fov) noexcept;
  float getOrthoSize() const noexcept;
  void setOrthoSize(float size) noexcept;
  float getNearPlane() const noexcept;
  void setNearPlane(float near) noexcept;
  float getFarPlane() const noexcept;
  void setFarPlane(float far) noexcept;
  float getNearPerspectivePlane() const noexcept;
  void setNearPerspectivePlane(float near) noexcept;
  float getFarPerspectivePlane() const noexcept;
  void setFarPerspectivePlane(float far) noexcept;
  float getNearOrthogonalPlane() const noexcept;
  void setNearOrthogonalPlane(float near) noexcept;
  float getFarOrthogonalPlane() const noexcept;
  void setFarOrthogonalPlane(float far) noexcept;
  
  Engine::ScriptShared::CameraMode getCameraMode() const noexcept;
  void setCameraMode(Engine::ScriptShared::CameraMode mode) noexcept;
  
  bool getDefaultMovement() const noexcept;
  void setDefaultMovement(bool state) noexcept;

  float getVelocity() const noexcept;
  void setVelocity(float velocity) noexcept;
  float getSensitivity() const noexcept;
  void setSensitivity(float sensitivity) noexcept;
  bool getMouseActive() const noexcept;
  void setMouseActive(bool active) noexcept;

//// ================== ////
//// ==== Movement ==== ////
//// ================== ////
public:
  void event(float delta_time);
  void resetMouse();

private:
  void cursorControl(float delta_time) noexcept;
  void velocityButtons(float delta_time) noexcept;
  void movementButtons(float delta_time, float& target_bank) noexcept;
  void rotationButtons(float delta_time, float& target_bank) noexcept;

};
};
