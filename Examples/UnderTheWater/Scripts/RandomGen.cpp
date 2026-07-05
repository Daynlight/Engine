// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#define SCRIPT_NAME Script_RandomGen
#define SCRIPT_FILE_NAME "RandomGen"
#define BUILDING_SCRIPT_DLL

#include "ScriptShared/ScriptShared/GameObjectScriptInterface.h"
#include <cstdio>
#include <math.h>
#include <deque>
#include <random>



namespace UW{
class SCRIPT_NAME : public GameObjectScriptInterface {
private:
  std::deque<glm::vec3> path;
  int interpolate_points_size = 0;
  glm::vec3 rotation_adjustment = glm::vec3(0.0f);
  int amount = 0;
  glm::vec3 diverse = glm::vec3(0, 0, 0);
  
  std::vector<std::string> child_object;
  std::string mesh = "TelescopeFish";

public:
  ~SCRIPT_NAME() = default;
  
  void OnLoad(){
    logger->info(SCRIPT_FILE_NAME, "Loaded");

    auto its1 = game_object_data->parameters.find("amount");
    if (its1 != game_object_data->parameters.end()) {
      if (auto* new_amount = std::get_if<int>(&its1->second)) {
        amount = *new_amount;
        logger->info(SCRIPT_FILE_NAME, "Amount = " + std::to_string(amount));
      };
    };


    auto its2 = game_object_data->parameters.find("mesh");
    if (its2 != game_object_data->parameters.end()) {
      if (auto* new_mesh = std::get_if<std::string>(&its2->second)) {
        mesh = *new_mesh;
        logger->info(SCRIPT_FILE_NAME, "Mesh = " + mesh);
      };
    };


    auto its3 = game_object_data->parameters.find("diverse");
    if (its3 != game_object_data->parameters.end()) {
      if (auto* str_ptr = std::get_if<glm::vec3>(&its3->second)) {
        glm::vec3 value = *str_ptr;
        diverse = value;
        logger->info(SCRIPT_FILE_NAME, "Diverse = [" + std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z) + "]");
      };
    };



    path.clear();
    auto itr = game_object_data->parameters.find("rotation_adjustment");
    if (itr != game_object_data->parameters.end()) {
      if (auto* new_rotation = std::get_if<glm::vec3>(&itr->second)) {
        rotation_adjustment = *new_rotation; 
        logger->info(SCRIPT_FILE_NAME, "Adjust rotation = [" + std::to_string(rotation_adjustment.x) + ", " + std::to_string(rotation_adjustment.y) + ", " + std::to_string(rotation_adjustment.z) + "]");
      };
    };



    interpolate_points_size = 0;
    auto it1 = game_object_data->parameters.find("p_size");
    if (it1 != game_object_data->parameters.end()) {
      if (auto* point_size = std::get_if<int>(&it1->second)) {
        interpolate_points_size = *point_size;
        logger->info(SCRIPT_FILE_NAME, "Position size = " + std::to_string(*point_size));
      };
    };

    
    glm::vec3 interpolate_value = glm::vec3(218.0f, 53, -31);
    auto it2 = game_object_data->parameters.find("p_0");
    if (it2 != game_object_data->parameters.end()) {
      if (auto* str_ptr = std::get_if<glm::vec3>(&it2->second)) {
        glm::vec3 value = *str_ptr;
          logger->info(SCRIPT_FILE_NAME, "Position[0] = [" + std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z) + "]");
      };
    };
    game_object_data->position = interpolate_value;
    path.push_back(interpolate_value);

    for(int i = 1; i <= interpolate_points_size; i++){
      auto it3 = game_object_data->parameters.find("p_" + std::to_string(i));
      if (it3 != game_object_data->parameters.end()) {
        if (auto* str_ptr = std::get_if<glm::vec3>(&it3->second)) {
          glm::vec3 value = *str_ptr;
          path.push_back(value);
          logger->info(SCRIPT_FILE_NAME, "Position[" + std::to_string(i) + "] = [" + std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z) + "]");
        };
      };
    };
  
    for(int i = 0; i < amount; i++){
      std::string new_child = game_object_data->name + "_child" + std::to_string(i);
      generateChild(new_child, i);
    };
  
  };

  void generateChild(const std::string& new_child, unsigned int i){
    child_object.emplace_back(new_child);
    object_manager->emplace_backObjectScript(new_child);

    GameObjectData* child_data = object_manager->getGameObjectDataObjectScript(new_child);
    child_data->mesh = mesh;
    child_data->rotation = game_object_data->rotation;
    child_data->scale = game_object_data->scale;
    child_data->shader = game_object_data->shader;
    child_data->textures = game_object_data->textures;
    child_data->materials = game_object_data->materials;
    child_data->hidden = true;

    child_data->parameters["rotation_adjustment"] = rotation_adjustment;
    child_data->parameters["p_size"] = int(path.size());

    for(int j = 0; j < path.size(); j++){
      int unique_seed = (j + 1) * i * 1337 + 2754;
      child_data->parameters["p_" + std::to_string(j)] = path[j] + randomVec3(unique_seed);
      if(j == 0) child_data->position = path[0] + randomVec3(unique_seed);
    }

    object_manager->addScriptObjectScript(new_child, "FishMovement");

    object_manager->saveRuntimeObjectScript(new_child);
  };



  glm::vec3 randomVec3(int seed) {
    unsigned int h = static_cast<unsigned int>(seed);
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    auto hashToFloat = [&](unsigned int val) {
      return static_cast<float>(val % 10000) / 10000.0f;
    };

    return glm::vec3(
      hashToFloat(h) * diverse.x - diverse.x / 2.0f, 
      hashToFloat(h * 37) * diverse.y - diverse.y / 2.0f, 
      hashToFloat(h * 131) * diverse.z - diverse.z / 2.0f
    );
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

