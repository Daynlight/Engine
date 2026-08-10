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



namespace Engine::Core{
class Object{
public:
  virtual void onLoad(Engine::Core::Scene& scene) = 0;
  virtual void onDestroy() = 0;
  virtual void onUpdate(float delta_time) = 0;
  virtual void onFixedUpdate(float fixed_delta_time, Engine::Core::Scene& scene) = 0;
  virtual void render(CW::Renderer::Renderer* renderer, Engine::ScriptShared::ICamera& culling_camera, Engine::ScriptShared::ICamera& render_camera, CW::Renderer::Uniform& shadows_uniform) = 0;

};
};
