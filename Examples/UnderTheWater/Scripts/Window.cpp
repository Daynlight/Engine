// Help me I'am Under The Water
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



namespace UW{
class SCRIPT_NAME : public GameObjectScriptInterface {
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
  };
  
  void OnRender(){
  };
  
  void OnDestroy(){
    glob_res->WINDOW_TITLE = base_name;
    logger->info(SCRIPT_FILE_NAME, "Destroyed");
  };
};
};



#ifndef PRODUCTION

extern "C" UW::GameObjectScriptInterface* SCRIPT_API GetScript() {
  UW::SCRIPT_NAME* script = new UW::SCRIPT_NAME();
  return (UW::GameObjectScriptInterface*)script;
};



extern "C" void SCRIPT_API DeleteScript(UW::GameObjectScriptInterface* script) {
  UW::SCRIPT_NAME* temp_script = (UW::SCRIPT_NAME*)script;
  delete temp_script;
};

#else

REGISTER_SCRIPT(SCRIPT_FILE_NAME, SCRIPT_NAME)

#endif

