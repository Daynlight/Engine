// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "Core.h"



Engine::Core::Core::Core()
  :scene(window)
{
  Engine::Utils::Logger::get().info("Core", "Core Initialized");
};



Engine::Core::Core::~Core(){
  Engine::Utils::Logger::get().info("Core", "Core Destroyed");
};



bool Engine::Core::Core::isRunning(){
  return !window.getWindowData()->should_close;
};



// ===================================== //
// ========== Core Operations ========== //
// ===================================== //
void Engine::Core::Core::onLoad(){
  Engine::Utils::Logger::get().info("Core", "Core Loading");

  DataSerializer::get().loadAll();
  initWindow();
  Engine::ScriptShared::GlobResource::get().input_data = window.getInputData();

  scene.onLoad();
  Engine::Utils::Logger::get().info("Core", "Scene Loaded");

  Engine::Utils::Logger::get().info("Core", "Core Loaded");
};



void Engine::Core::Core::onDestroy() {
  Engine::Utils::Logger::get().info("Core", "Destroying Core");


#ifndef PRODUCTION
  DataSerializer::get().saveAll();
  Engine::Utils::Logger::get().info("Scene", "Force saved scene data");
#endif

  scene.onDestroy();
  Engine::Utils::Logger::get().info("Core", "Scene Destroyed");
  
  Engine::Core::Resources::get().destroy();
  Engine::Utils::Logger::get().info("Core", "Resources Destroyed");

  Engine::Utils::Logger::get().info("Core", "Core Destroyed");
};



void Engine::Core::Core::render(){
  scene.render();
};



CW::Renderer::Framebuffer& Engine::Core::Core::get_fbo(){
// #ifndef PRODUCTION
//   if(scene.post_processing_on) return scene.post_fbo;
//   else return scene.fbo; 
// #else
// #endif
  return scene.getFbo();
};



void Engine::Core::Core::swapFrame(){
  window.windowEvents();
  window.swapBuffer();
};



void Engine::Core::Core::update(){
#ifndef PRODUCTION
  swapCamera();
#endif

  scene.onUpdate(window.getWindowData()->delta_time);
};



void Engine::Core::Core::fixedUpdate(){
  fixed_update_time_acc += window.getWindowData()->delta_time;

  if(Engine::ScriptShared::GlobResource::get().FIXED_HZ > Engine::Config::MAX_FIXED_HZ) Engine::ScriptShared::GlobResource::get().FIXED_HZ = Engine::Config::MAX_FIXED_HZ;
  if(Engine::ScriptShared::GlobResource::get().FIXED_HZ < Engine::Config::MIN_FIXED_HZ) Engine::ScriptShared::GlobResource::get().FIXED_HZ = Engine::Config::MIN_FIXED_HZ;
  
  float fixed_time_step = 1.0f / Engine::ScriptShared::GlobResource::get().FIXED_HZ;
  
  int max_steps = Engine::Config::MAX_FIXED_STEPS;
  while(fixed_update_time_acc >= fixed_time_step && max_steps-- > 0){
    if(cached_title != Engine::ScriptShared::GlobResource::get().WINDOW_TITLE) updateTitle();
    if(cached_vsync != Engine::ScriptShared::GlobResource::get().VSYNC) updateVsync();
    
    scene.onFixedUpdate(fixed_time_step);

    fixed_update_time_acc -= fixed_time_step;
  };

  if(max_steps <= 0) fixed_update_time_acc = 0;
};



// ============================= //
// ========== Helpers ========== //
// ============================= //
void Engine::Core::Core::initWindow(){
  Engine::Utils::Logger::get().info("Core", "Window Initialization");

  updateTitle();

  window.setCursorVisibility(Engine::Config::DEFAULT_CURSOR_IS_VISIBLE);
  Engine::Utils::Logger::get().info("Core", "Cursor visiblity set to - " + std::string(Engine::Config::DEFAULT_CURSOR_IS_VISIBLE == 1 ? "On" : "Off"));

  updateVsync();

  Engine::Utils::Logger::get().info("Core", "Window Initialized");
};



void Engine::Core::Core::updateTitle(){
  cached_title = Engine::ScriptShared::GlobResource::get().WINDOW_TITLE;

  window.setWindowTitle(cached_title);
  Engine::Utils::Logger::get().info("Core", "Title set to - " + cached_title);
};



void Engine::Core::Core::updateVsync(){
  cached_vsync = Engine::ScriptShared::GlobResource::get().VSYNC;

  window.setVsync(cached_vsync);
  Engine::Utils::Logger::get().info("Core", "VSync set to - " + std::string(cached_vsync != 0 ? "On" : "Off"));
};



#ifndef PRODUCTION
void Engine::Core::Core::swapCamera(){
  if(window.getInputData()->is_key_down(Engine::Config::SWAP_CAMERA_BTN) && camera_swap_cooldown_acc <= 0.0f) {
    scene.debug_camera_on = !scene.debug_camera_on;
    camera_swap_cooldown_acc = Engine::Config::CAMERA_SWAP_COOLDOWN;

    Engine::Utils::Logger::get().info("Core", "Camera Core to { "+ std::string(scene.debug_camera_on ? "DEBUG CAMERA" : "NORMAL CAMERA") + " }");
  };

  if(camera_swap_cooldown_acc >= 0.0f) camera_swap_cooldown_acc -= window.getWindowData()->delta_time;
};
#endif
