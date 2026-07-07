// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include "Renderer.h"

#include <vector>

#include "Camera/Camera.h"
#include "Objects/Object.h"
#include "Objects/GameObject.h"
#include "Objects/ObjectManager.h"
#include "DataSerializer/DataSerializer.h"
#include "Utils/Resource/Resource.h"



namespace UW{
class Scene{
#ifndef PRODUCTION
public:
#else
private:
#endif
  CW::Renderer::Renderer& window;
  CW::Renderer::Framebuffer fbo;
  CW::Renderer::Framebuffer shadows_fbo;
  UW::Camera camera;
  
#ifndef PRODUCTION
  UW::Camera debug_camera;
  bool debug_camera_on = UW::Config::DEFAULT_DEBUG_CAMERA_ON;
  bool post_processing_on = UW::Config::DEFAULT_POST_PROCESSING_ON;
  bool shadows_on = UW::Config::DEFAULT_SHADOWS_ON;
  
  float save_acc = 0.0f;
  #endif
  
  UW::Camera light_camera;
  CW::Renderer::Uniform shadows_uniform_on;
  CW::Renderer::Uniform shadows_uniform_off;
  glm::mat4 light_space_matrix;
  glm::vec3 last_light_camera_pos = glm::vec3(0.0f);
  glm::vec3 last_light_camera_dir = glm::vec3(0.0f);
  float last_light_camera_fov = 1.0f;

  UW::Resource<CW::Renderer::Mesh> screen_quad;
  CW::Renderer::Uniform post_uniform;

public:
  Scene(CW::Renderer::Renderer& window);
  ~Scene();
  
  void onLoad();
  void onDestroy();
  void onUpdate(float delta_time);
  void onFixedUpdate(float fixed_delta_time);
  void render();

private:
  void postProcessing();
  void compileShadows();
  void renderFrame(UW::Camera& camera);

};
};
