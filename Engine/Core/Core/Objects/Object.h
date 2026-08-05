// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include "Renderer.h"

#include "../Camera/Camera.h"



namespace Engine::Core{
  class Scene;
};



namespace Engine{
class Object{
public:
  virtual void onLoad(Engine::Core::Scene& scene) = 0;
  virtual void onDestroy() = 0;
  virtual void onUpdate(float delta_time) = 0;
  virtual void onFixedUpdate(float fixed_delta_time, Engine::Core::Scene& scene) = 0;
  virtual void render(CW::Renderer::Renderer* renderer, ICamera& culling_camera, ICamera& render_camera, CW::Renderer::Uniform& shadows_uniform) = 0;

};
};
