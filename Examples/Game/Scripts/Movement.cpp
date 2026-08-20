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



  float height_collision(const std::string& name){
    Engine::ScriptShared::GameObjectData* collider = engine.object_manager->getGameObjectData(name);
    if(collider == nullptr){
      engine.logger->erro("height_collision", "nullptr");
      return 0.0f;
    };

    float height_z = 0;
    auto its1 = collider->parameters.find("height");
    if (its1 != collider->parameters.end()) {
      if (auto* new_amount = std::get_if<float>(&its1->second)) {
        height_z = *new_amount;
      };
    };

    glm::vec3 position = object_data->position - glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 size = object_data->scale;

    glm::vec3 collider_pos = collider->position;
    glm::vec3 collider_size = collider->scale;

    bool inside_x = std::abs(position.x - collider_pos.x) < (size.x + collider_size.x) * 0.5f;
    bool inside_y = std::abs(position.y - collider_pos.y) < (size.y + collider_size.y) * 0.5f;

    if (inside_x && inside_y) {
      float stair_left_y = collider_pos.y - (collider_size.y * 0.5f);
      float progress = (position.y - stair_left_y) / collider_size.y;
      progress = glm::clamp(progress, 0.0f, 1.0f);

      float stair_bottom_y = collider_pos.y - (collider_size.y * 0.5f);
      return (height_z + (progress * collider_size.y)) * 5.0f;
    };
    
    return 0.0f;
  };

  float height(){
    std::vector<std::string> colliders = {"Stairs", "Stairs2", "Stairs3"};
    float height_val = 0.0f;
    for(const std::string& name : colliders) height_val += height_collision(name);
    return height_val;
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
