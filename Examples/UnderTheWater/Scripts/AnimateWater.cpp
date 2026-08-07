// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#define SCRIPT_NAME Script_AnimationWater
#define SCRIPT_FILE_NAME "AnimationWater"
#define BUILDING_SCRIPT_DLL

#include "ScriptShared/ScriptShared/GameObjectScriptInterface.h"
#include <cstdio>
#include <math.h>
#include <deque>



namespace Engine{
class SCRIPT_NAME : public Engine::ScriptShared::GameObjectScriptInterface {
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



REGISTER_SCRIPT(SCRIPT_FILE_NAME, SCRIPT_NAME)
