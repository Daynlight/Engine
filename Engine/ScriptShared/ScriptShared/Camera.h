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



namespace Engine {
class ICamera {
public:
  virtual void rotate(float xoffset, float yoffset, float zoffset) = 0;
  virtual void updateDirection() = 0;
  
  virtual glm::mat4 transformation() = 0;
  virtual glm::mat4 projection() = 0;
  virtual glm::mat4 view() = 0;
  
  virtual glm::vec3 getPosition() = 0;
  virtual void setPosition(glm::vec3 position) = 0;
  virtual glm::vec3 getDirection() = 0;
  virtual void setDirection(glm::vec3 direction) = 0;
  
  virtual void event() = 0;
  virtual void resetMouse() = 0;
  virtual void setOrthographic(bool enable) = 0;

};
};
