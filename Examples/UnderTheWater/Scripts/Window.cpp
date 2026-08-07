// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#define SCRIPT_NAME Script_Window
#define SCRIPT_FILE_NAME "Window"
#define BUILDING_SCRIPT_DLL

#include "ScriptShared/ScriptShared/GameObjectScriptInterface.h"
#include <cstdio>
#include <math.h>
#include <deque>



namespace Engine{
class SCRIPT_NAME : public Engine::ScriptShared::GameObjectScriptInterface {
private:
  std::string base_name = "Under The Water";
  unsigned int samples = 0;
  float acc_time = 0.0f;

public:
  ~SCRIPT_NAME() = default;
  
  void OnLoad(){
    glob_res->WINDOW_TITLE = base_name;
    logger->info(SCRIPT_FILE_NAME, "Loaded");
  };
  
  void OnUpdate(float delta_time){
    if(samples > 200){
      float fps = samples / acc_time;
      glob_res->WINDOW_TITLE = base_name + " | " + std::to_string(floor(fps)) + " fps";
      acc_time = 0.0f;
      samples = 0;
    }
    else{
      acc_time += delta_time;
      samples++;
    }
  };
  
  void OnFixedUpdate(float fixed_delta_time){
    Engine::ICamera& camera = camera_controller->getActiveCamera();
    glm::vec3 ps = camera.getPosition();
    ps += glm::vec3(0.0, 1.0, 0.0) * fixed_delta_time;
    camera.setPosition(ps);
  };
  
  void OnRender(){
  };
  
  void OnDestroy(){
    glob_res->WINDOW_TITLE = base_name;
    logger->info(SCRIPT_FILE_NAME, "Destroyed");
  };
};
};



REGISTER_SCRIPT(SCRIPT_FILE_NAME, SCRIPT_NAME)
