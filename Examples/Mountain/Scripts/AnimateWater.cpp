// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#define SCRIPT_NAME Script_Template
#define SCRIPT_FILE_NAME "Template"
#define BUILDING_SCRIPT_DLL

#include "ScriptShared/ScriptShared/GameObjectScriptInterface.h"
#include <cstdio>
#include <math.h>
#include <deque>



namespace UW{
class SCRIPT_NAME : public GameObjectScriptInterface {
private:
  float elapsed_time = 0.0f;
public:
  ~SCRIPT_NAME() = default;
  
  void OnLoad(){
    logger->info(SCRIPT_FILE_NAME, "Loaded");
  };
  
  void OnUpdate(float delta_time){
    elapsed_time += delta_time;
  };
  
  void OnFixedUpdate(float fixed_delta_time){
  };
  
  void OnRender(){
    game_object_data->uniforms["time"] = elapsed_time;
  };
  
  void OnDestroy(){
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

