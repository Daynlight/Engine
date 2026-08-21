// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#define SCRIPT_NAME Script_Template
#define SCRIPT_FILE_NAME "Template"
#define BUILDING_SCRIPT_DLL

#include "Engine/ScriptShared/ScriptShared/GameObjectScriptInterface.h"
#include <cstdio>
#include <math.h>
#include <deque>



namespace Engine{
class SCRIPT_NAME : public Engine::ScriptShared::GameObjectScriptInterface {
private:

public:
  ~SCRIPT_NAME() = default;
  
  void OnLoad(){
    engine.logger->info(SCRIPT_FILE_NAME, "Loaded");
  };
  
  void OnUpdate(float delta_time){
  };
  
  void OnFixedUpdate(float fixed_delta_time){
  };
  
  void OnRender(){
  };
  
  void OnDestroy(){
    engine.logger->info(SCRIPT_FILE_NAME, "Destroyed");
  };
};
};



REGISTER_SCRIPT(SCRIPT_FILE_NAME, SCRIPT_NAME)
