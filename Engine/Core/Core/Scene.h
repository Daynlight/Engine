// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include "Renderer.h"

#include <vector>

#include "Camera/CameraController.h"
#include "Objects/Object.h"
#include "Objects/GameObject.h"
#include "Objects/ObjectManager.h"
#include "DataSerializer/DataSerializer.h"
#include "Utils/Resource/Resource.h"



namespace Engine::Core{
class Scene{
public:
  Engine::Core::CameraController camera_controller;

#ifndef PRODUCTION
public:
#else
private:
#endif
  CW::Renderer::Renderer& window;
  
#ifndef PRODUCTION
  Engine::Core::Camera debug_camera;
  bool debug_camera_on = Engine::Config::DEFAULT_DEBUG_CAMERA_ON;
  
  float save_acc = 0.0f;
#endif

public:
  Scene(CW::Renderer::Renderer& window);
  ~Scene();
  
  void onLoad();
  void onDestroy();
  void onUpdate(float delta_time);
  void onFixedUpdate(float fixed_delta_time);
  void render();
  CW::Renderer::Framebuffer& getFbo();

private:
  void renderFrame(Engine::Core::Camera& camera);
};
};
