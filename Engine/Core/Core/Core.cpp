// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "Core.h"



#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
extern "C" {
  __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif



UW::Core::Core()
  :scene(window)
{
  Logger::get().info("Core", "Core Initialized");
};



UW::Core::~Core(){
  Logger::get().info("Core", "Core Destroyed");
};



bool UW::Core::isRunning(){
  return !window.getWindowData()->should_close;
};



// ===================================== //
// ========== Core Operations ========== //
// ===================================== //
void UW::Core::onLoad(){
  Logger::get().info("Core", "Core Loading");

  DataSerializer::get().loadAll();
  initWindow();
  UW::GlobResource::get().input_data = window.getInputData();

  scene.onLoad();
  Logger::get().info("Core", "Scene Loaded");

  Logger::get().info("Core", "Core Loaded");
};



void UW::Core::onDestroy() {
  Logger::get().info("Core", "Destroying Core");


#ifndef PRODUCTION
  DataSerializer::get().saveAll();
  Logger::get().info("Scene", "Force saved scene data");
#endif

  scene.onDestroy();
  Logger::get().info("Core", "Scene Destroyed");
  
  Resources::get().destroy();
  Logger::get().info("Core", "Resources Destroyed");

  Logger::get().info("Core", "Core Destroyed");
};



void UW::Core::render(){
  scene.render();
};



void UW::Core::swapFrame(){
  window.windowEvents();
  window.swapBuffer();
};



void UW::Core::update(){
#ifndef PRODUCTION
  swapCamera();
#endif

  scene.onUpdate(window.getWindowData()->delta_time);
};



void UW::Core::fixedUpdate(){
  fixed_update_time_acc += window.getWindowData()->delta_time;

  if(UW::GlobResource::get().FIXED_HZ > UW::Config::MAX_FIXED_HZ) UW::GlobResource::get().FIXED_HZ = UW::Config::MAX_FIXED_HZ;
  if(UW::GlobResource::get().FIXED_HZ < UW::Config::MIN_FIXED_HZ) UW::GlobResource::get().FIXED_HZ = UW::Config::MIN_FIXED_HZ;
  
  float fixed_time_step = 1.0f / UW::GlobResource::get().FIXED_HZ;
  
  int max_steps = UW::Config::MAX_FIXED_STEPS;
  while(fixed_update_time_acc >= fixed_time_step && max_steps-- > 0){
    if(cached_title != UW::GlobResource::get().WINDOW_TITLE) updateTitle();
    if(cached_vsync != UW::GlobResource::get().VSYNC) updateVsync();
    
    scene.onFixedUpdate(fixed_time_step);

    fixed_update_time_acc -= fixed_time_step;
  };

  if(max_steps <= 0) fixed_update_time_acc = 0;
};



// ============================= //
// ========== Helpers ========== //
// ============================= //
void UW::Core::initWindow(){
  Logger::get().info("Core", "Window Initialization");

  updateTitle();

  window.setCursorVisibility(UW::Config::DEFAULT_CURSOR_IS_VISIBLE);
  Logger::get().info("Core", "Cursor visiblity set to - " + std::string(UW::Config::DEFAULT_CURSOR_IS_VISIBLE == 1 ? "On" : "Off"));

  updateVsync();

  Logger::get().info("Core", "Window Initialized");
};



void UW::Core::updateTitle(){
  cached_title = UW::GlobResource::get().WINDOW_TITLE;

  window.setWindowTitle(cached_title);
  Logger::get().info("Core", "Title set to - " + cached_title);
};



void UW::Core::updateVsync(){
  cached_vsync = UW::GlobResource::get().VSYNC;

  window.setVsync(cached_vsync);
  Logger::get().info("Core", "VSync set to - " + std::string(cached_vsync != 0 ? "On" : "Off"));
};



#ifndef PRODUCTION
void UW::Core::swapCamera(){
  if(window.getInputData()->is_key_down(UW::Config::SWAP_CAMERA_BTN) && camera_swap_cooldown_acc <= 0.0f) {
    scene.debug_camera_on = !scene.debug_camera_on;
    camera_swap_cooldown_acc = UW::Config::CAMERA_SWAP_COOLDOWN;

    Logger::get().info("Core", "Camera SwCoreed to { "+ std::string(scene.debug_camera_on ? "DEBUG CAMERA" : "NORMAL CAMERA") + " }");
  };

  if(camera_swap_cooldown_acc >= 0.0f) camera_swap_cooldown_acc -= window.getWindowData()->delta_time;
};
#endif
