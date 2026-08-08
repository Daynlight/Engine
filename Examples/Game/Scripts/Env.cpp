// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#define SCRIPT_NAME Script_Env
#define SCRIPT_FILE_NAME "Env"
#define BUILDING_SCRIPT_DLL

#include "ScriptShared/ScriptShared/GameObjectScriptInterface.h"
#include <cstdio>
#include <math.h>
#include <deque>



namespace Engine{
class SCRIPT_NAME : public Engine::ScriptShared::GameObjectScriptInterface {
private:

public:
  ~SCRIPT_NAME() = default;

  std::vector<std::string> child_object;
  
  void OnLoad(){
    logger->info(SCRIPT_FILE_NAME, "Loaded");
    for(int i = 0; i < 20; i++){
      for(int j = 0; j < 20; j++){
        generateChild("child" + std::to_string(i) + "x"+ std::to_string(j), i, j); 
      };
    };
  };

  void generateChild(const std::string& new_child, unsigned int i, unsigned int j){
    child_object.emplace_back(new_child);
    object_manager->emplace_backObjectScript(new_child);

    Engine::ScriptShared::GameObjectData* child_data = object_manager->getGameObjectDataObjectScript(new_child);
    child_data->mesh = game_object_data->mesh;
    child_data->position = game_object_data->position + glm::vec3(2.0f * game_object_data->scale.x  * i, 2.0f * game_object_data->scale.y * j, 0.0f);
    child_data->rotation = game_object_data->rotation;
    child_data->scale = game_object_data->scale;
    child_data->shader = game_object_data->shader;
    child_data->textures = game_object_data->textures;
    child_data->uniforms = game_object_data->uniforms;
    child_data->gl_nearest = game_object_data->gl_nearest;
    child_data->culling_on = game_object_data->culling_on;
    child_data->hidden = true;


    logger->info(SCRIPT_FILE_NAME, "Spawned");
    object_manager->saveRuntimeObjectScript(new_child);
  };
  
  void OnUpdate(float delta_time){
  };
  
  void OnFixedUpdate(float fixed_delta_time){
  };
  
  void OnRender(){
  };
  
  void OnDestroy(){
    for(std::string child : child_object){
      object_manager->eraseObjectScript(child);
    };

    logger->info(SCRIPT_FILE_NAME, "Destroyed");
  };
};
};



REGISTER_SCRIPT(SCRIPT_FILE_NAME, SCRIPT_NAME)
