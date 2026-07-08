// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "App.h"



#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
extern "C" {
  __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif



UW::App::App()
#ifndef PRODUCTION
  : editor(core, fps)
#endif
{
  Engine::Utils::Logger::get().info("App", "App Initialization");
  onLoad();
  Engine::Utils::Logger::get().info("App", "App Initialized");
};



UW::App::~App(){
  Engine::Utils::Logger::get().info("App", "App Destroying");
  onDestroy();
  Engine::Utils::Logger::get().info("App", "App Destroyed");
};



bool UW::App::isRunning(){
  return core.isRunning();
};



void UW::App::run(){  
  update();
  fixedUpdate();
  render();
};



// ===================================== //
// ========== Core Operations ========== //
// ===================================== //
void UW::App::onLoad(){
  Engine::Utils::Logger::get().info("App", "App Loading");

#ifndef PRODUCTION
  editor.onLoad();
#endif

  core.onLoad();
  Engine::Utils::Logger::get().info("App", "Scene Loaded");

  Engine::Utils::Logger::get().info("App", "App Loaded");
};



void UW::App::onDestroy() {
  Engine::Utils::Logger::get().info("App", "Destroying App");


#ifndef PRODUCTION
  editor.onDestroy();
  Engine::Utils::Logger::get().info("App", "UI Destroyed");
#endif

  core.onDestroy();
  Engine::Utils::Logger::get().info("App", "Core Destroyed");
  
  Engine::Utils::Logger::get().info("App", "App Destroyed");

#ifndef PRODUCTION
  Engine::Utils::Logger::get().info("App", "Recorded AVG FPS = " + std::to_string(total_fps_acc / total_fps_id));
#endif
};



void UW::App::render(){
  core.render();

#ifndef PRODUCTION
  editor.render();
#endif

  core.swapFrame();
};




void UW::App::update(){
#ifndef PRODUCTION
  updateFps();
#endif

  core.update();
};



void UW::App::fixedUpdate(){
#ifndef PRODUCTION
  fixed_update_time_acc += core.window.getWindowData()->delta_time;

  float fixed_time_step = 1.0f / UW::GlobResource::get().FIXED_HZ;
  
  int max_steps = Engine::Config::MAX_FIXED_STEPS;
  while(fixed_update_time_acc >= fixed_time_step && max_steps-- > 0){
    guiSettings.window_width = core.window.getWindowData()->width;
    guiSettings.window_height = core.window.getWindowData()->height;
    
    fixed_update_time_acc -= fixed_time_step;
  };
  if(max_steps <= 0) fixed_update_time_acc = 0;
#endif
  
  core.fixedUpdate();
};




#ifndef PRODUCTION
void UW::App::updateFps(){
  if(fps_id > Engine::Config::FPS_SAMPLES){
    fps = fps_id / fps_acc;
    fps_acc = 0.0f;
    fps_id = 0;
    total_fps_acc += fps;
    total_fps_id++;
  }
  else{
    fps_acc += core.window.getWindowData()->delta_time;
    fps_id++;
  };
};
#endif
