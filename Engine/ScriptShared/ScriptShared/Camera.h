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



namespace Engine::ScriptShared{
enum CameraMode{
  PERSPECTIVE = 0,
  ORTHOGONAL = 1
};



class ICamera {
public:
  virtual glm::mat4 transformation() noexcept = 0;
  virtual glm::mat4 view() noexcept = 0;
  virtual glm::mat4 projection() noexcept = 0;
  
  virtual glm::vec3 getPosition() const noexcept = 0;
  virtual void setPosition(glm::vec3 position) noexcept = 0;
  virtual glm::vec3 getDirection() const noexcept = 0;
  virtual void setDirection(glm::vec3 direction) noexcept = 0;
  
  virtual float getFov() const noexcept = 0;
  virtual void setFov(float fov) noexcept = 0;
  virtual float getOrthoSize() const noexcept = 0;
  virtual void setOrthoSize(float size) noexcept = 0;
  virtual float getNearPlane() const noexcept = 0;
  virtual void setNearPlane(float near) noexcept = 0;
  virtual float getFarPlane() const noexcept = 0;
  virtual void setFarPlane(float far) noexcept = 0;
  virtual float getNearPerspectivePlane() const noexcept = 0;
  virtual void setNearPerspectivePlane(float near) noexcept = 0;
  virtual float getFarPerspectivePlane() const noexcept = 0;
  virtual void setFarPerspectivePlane(float far) noexcept = 0;
  virtual float getNearOrthogonalPlane() const noexcept = 0;
  virtual void setNearOrthogonalPlane(float near) noexcept = 0;
  virtual float getFarOrthogonalPlane() const noexcept = 0;
  virtual void setFarOrthogonalPlane(float far) noexcept = 0;
  
  
  virtual Engine::ScriptShared::CameraMode getCameraMode() const noexcept = 0;
  virtual void setCameraMode(Engine::ScriptShared::CameraMode mode) noexcept = 0;
  
  virtual bool getDefaultMovement() const noexcept = 0;
  virtual void setDefaultMovement(bool state) noexcept = 0;

  virtual float getVelocity() const noexcept = 0;
  virtual void setVelocity(float velocity) noexcept = 0;
  virtual float getSensitivity() const noexcept = 0;
  virtual void setSensitivity(float sensitivity) noexcept = 0;
  virtual bool getMouseActive() const noexcept = 0;
  virtual void setMouseActive(bool active) noexcept = 0;

  virtual void event(float delta_time) = 0;
  virtual void resetMouse() = 0;

};
};
