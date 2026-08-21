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

  std::vector<std::string> colliders = {"Stairs", "Stairs2", "Stairs3"};
  const float STAIRS_HEIGHT_COFF = 4.0f;
  const float STAIRS_ENTER_EPS = 4.0f;
  float zoom = DEFAULT_ZOOM;
  float height = 0.0f;

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



  // ================= //
  // ==== Physics ==== //
  // ================= //
  std::array<glm::vec3, 2> getPlayerCollider(){
    glm::vec3 player_size = object_data->scale / 2.0f;
    glm::vec3 player_pos = object_data->position - glm::vec3(0.0f, 1.0f, 0.0f);

    return {player_pos, player_size};
  };

  std::array<glm::vec3, 2> getObjectCollider(const std::string& name){
    Engine::ScriptShared::GameObjectData* collider = engine.object_manager->getGameObjectData(name);
    if (collider == nullptr) {
      engine.logger->erro(SCRIPT_FILE_NAME, "collider " + name + " is nullptr");
      return {glm::vec3(0.0f), glm::vec3(0.0f)};
    };

    glm::vec3 collider_size = collider->scale;
    glm::vec3 collider_pos = collider->position;
    collider_pos.y += collider_size.y / 2.0f;

    return {collider_pos, collider_size};
  };

  glm::vec3 collision(const std::string& name) {
    if (object_data == nullptr) {
      engine.logger->erro(SCRIPT_FILE_NAME, "object_data is nullptr");
      return glm::vec3(0.0f);
    };

    std::array<glm::vec3, 2> player_collider = getPlayerCollider();
    std::array<glm::vec3, 2> object_collider = getObjectCollider(name);

    float overlap_x = (player_collider[1].x + object_collider[1].x) - std::abs(player_collider[0].x - object_collider[0].x);
    float overlap_y = (player_collider[1].y + object_collider[1].y) - std::abs(player_collider[0].y - object_collider[0].y);

    if (overlap_x > 0.0f && overlap_y > 0.0f) {
      if (overlap_x < overlap_y) {
        float sign_x = (player_collider[0].x < object_collider[0].x) ? -1.0f : 1.0f;
        return glm::vec3(overlap_x * sign_x, 0.0f, 0.0f);
      } else {
        float sign_y = (player_collider[0].y < object_collider[0].y) ? -1.0f : 1.0f;
        return glm::vec3(0.0f, overlap_y * sign_y, 0.0f);
      };
    };

    return glm::vec3(0.0f);
  };

  float stairs_height(const std::string& name){
    Engine::ScriptShared::GameObjectData* collider = engine.object_manager->getGameObjectData(name);
    if(collider == nullptr){
      engine.logger->erro("height_collision", "nullptr");
      return 0.0f;
    };

    float min_height = 0.0f;
    float max_height = 0.0f;
    auto its1 = collider->parameters.find("min_height");
    if (its1 != collider->parameters.end()) {
      if (auto* new_amount = std::get_if<float>(&its1->second)) min_height = *new_amount;
    };
    auto its2 = collider->parameters.find("max_height");
    if (its2 != collider->parameters.end()) {
      if (auto* new_amount = std::get_if<float>(&its2->second)) max_height = *new_amount;
    };

    std::array<glm::vec3, 2> player_collider = getPlayerCollider();
    std::array<glm::vec3, 2> object_collider = getObjectCollider(name);

    float stair_start_y = object_collider[0].y - (object_collider[1].y * 0.5f);
    float progress = (player_collider[0].y - stair_start_y) / object_collider[1].y;
    progress = glm::clamp(progress, 0.0f, 1.0f);

    return (min_height + progress * (max_height - min_height)) * STAIRS_HEIGHT_COFF;
  };

  void physics(){
    float height_val = 0.0f;
    glm::vec3 collision_dir = glm::vec3(0.0f);
    
    for(const std::string& name : colliders) {
      glm::vec3 new_collision = collision(name);
      collision_dir += new_collision;
      
      if(new_collision != glm::vec3(0.0f)){
        float new_height_val = stairs_height(name);
        if(new_height_val > height_val) height_val = new_height_val;
      };
    };
    
    if(std::abs(height - height_val) < STAIRS_ENTER_EPS) height = height_val;
    else object_data->position += collision_dir;
  };



  // ================== //
  // ==== Movement ==== //
  // ================== //
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
    
    physics();
  };

  void cameraTracking(){
    Engine::ScriptShared::ICamera& camera = engine.camera_controller->getActiveCamera();
    
    glm::vec3 position = object_data->position;

    position.z = 10.0f;
    camera.setPosition(position);
    
    camera.setOrthoSize(zoom + height);
    camera.setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
    camera.setCameraMode(Engine::ScriptShared::CameraMode::ORTHOGONAL);
  };
};
};



REGISTER_SCRIPT(SCRIPT_FILE_NAME, SCRIPT_NAME)
