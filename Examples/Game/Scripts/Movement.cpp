// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#define SCRIPT_NAME Script_Movement
#define SCRIPT_FILE_NAME "Movement"
#define BUILDING_SCRIPT_DLL

#include "Engine/ScriptShared/ScriptShared/GameObjectScriptInterface.h"
#include <cstdio>
#include <math.h>
#include <deque>
#include "Config.h"



namespace Engine{
class SCRIPT_NAME : public Engine::ScriptShared::GameObjectScriptInterface {
private:
  float zoom = DEFAULT_ZOOM;

public:
  ~SCRIPT_NAME() = default;
  
  void OnLoad(){
    engine.logger->info(SCRIPT_FILE_NAME, "Loaded");
  };
  
  void OnUpdate(float delta_time){
  };
  
  void OnFixedUpdate(float fixed_delta_time){
    movement(fixed_delta_time);
    cameraTracking();
  };
  
  void OnRender(){
  };
  
  void OnDestroy(){
    engine.logger->info(SCRIPT_FILE_NAME, "Destroyed");
  };




  float height(){
    glm::vec3 position = object_data->position;
    Engine::ScriptShared::GameObjectData* collider = engine.object_manager->getGameObjectData("Stairs");
    
    if(collider == nullptr) return 0.0f;
    if(std::abs(position.x - collider->position.x) < 1.0f
      && std::abs(position.y * 2.0f - collider->position.y) < 1.0f)
      return 5.0f;
    return 0.0f;
  };

  void movement(float delta_time){
    glm::vec3 delta_movement = glm::vec3(0.0f);
    bool shift_pressed = engine.glob_res->input_data->is_key_down("LSHIFT");

    float target_zoom = shift_pressed ? DEFAULT_SPRINT_ZOOM : DEFAULT_ZOOM;
    zoom += (target_zoom - zoom) * DEFAULT_ZOOM_ACCELERATION * delta_time;

    if(engine.glob_res->input_data->is_key_down("W")) delta_movement.y += 1;
    if(engine.glob_res->input_data->is_key_down("S")) delta_movement.y -= 1;
    if(engine.glob_res->input_data->is_key_down("D")) delta_movement.x += 1;
    if(engine.glob_res->input_data->is_key_down("A")) delta_movement.x -= 1;
    if(delta_movement.x == 0.0f && delta_movement.y == 0.0f) return;
    
    glm::vec3 move_vector = glm::normalize(delta_movement);

    if(shift_pressed) move_vector = move_vector * DEFAULT_SPRINT_SPEED * delta_time;
    else move_vector = move_vector * DEFAULT_SPEED * delta_time;
    
    object_data->position += move_vector;
    
    // engine.logger->warn(SCRIPT_FILE_NAME, std::to_string(move_vector.x) + ", " + std::to_string(move_vector.y));
  };

  void cameraTracking(){
    Engine::ScriptShared::ICamera& camera = engine.camera_controller->getActiveCamera();
    
    glm::vec3 position = object_data->position;

    position.z = 10.0f;
    camera.setPosition(position);
    
    camera.setOrthoSize(zoom + height());
    camera.setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
    camera.setCameraMode(Engine::ScriptShared::CameraMode::ORTHOGONAL);
  };
};
};



REGISTER_SCRIPT(SCRIPT_FILE_NAME, SCRIPT_NAME)
