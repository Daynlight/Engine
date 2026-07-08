// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "ObjectManager.h"



UW::ObjectManager &UW::ObjectManager::get(){
  static ObjectManager instance;
  return instance; 
};



void UW::ObjectManager::emplace_back(const std::string &name){
  objects.emplace_back(UW::GameObject(name, "empty", "Default"));
};



void UW::ObjectManager::erase(const std::string &name) {
  for (auto it = objects.begin(); it != objects.end(); ) {
    if (it->game_object_data.name == name) {
      it->onDestroy();
      it->scripts.clear(); 
      // it->mesh_id = -1;
      it = objects.erase(it);
      
    } else {
      ++it; 
    };
  };
};



UW::GameObjectData *UW::ObjectManager::getGameObjectData(const std::string &name){
  for(auto& object : objects)
    if(object.game_object_data.name == name)
      return &object.copy_game_object_data;

  return nullptr;
};



void UW::ObjectManager::addScript(const std::string &object_name, const std::string &path){
  for (auto& obj : objects) {
    if (obj.game_object_data.name == object_name) {
      obj.scripts.emplace_back(path);
      return;
    };
  };
  Engine::Utils::Logger::get().erro("ObjectManager", "Could not find object: " + object_name);
};



void UW::ObjectManager::removeScript(const std::string &object_name, const std::string &path) {
  for (auto& obj : objects) {
    if (obj.game_object_data.name == object_name) {
      obj.scripts.erase(
        std::remove_if(obj.scripts.begin(), obj.scripts.end(), 
          [&](const GameObjectScriptRecord& record) {
            return record.getPath() == path;
          }), 
        obj.scripts.end()
      );
      return;
    };
  };
  Engine::Utils::Logger::get().erro("ObjectManager", "Could not find object: " + object_name);
};



void UW::ObjectManager::saveRuntime(const std::string& object_name){
  for (auto& obj : objects) {
    if (obj.game_object_data.name == object_name) {
      obj.game_object_data = obj.copy_game_object_data;
    };
  };
};



void UW::ObjectManager::emplace_backObjectScript(const std::string &name){
  script_objects.emplace_back(UW::GameObject(name, "empty", "Default"));
};



void UW::ObjectManager::eraseObjectScript(const std::string &name) {
  for (auto it = script_objects.begin(); it != script_objects.end(); ) {
    if (it->game_object_data.name == name) {
      it->onDestroy();
      it->scripts.clear(); 
      // it->mesh_id = -1;
      it = script_objects.erase(it);
      
    } else {
      ++it; 
    };
  };
};



UW::GameObjectData *UW::ObjectManager::getGameObjectDataObjectScript(const std::string &name){
  for(auto& object : script_objects)
    if(object.game_object_data.name == name)
      return &object.copy_game_object_data;

  return nullptr;
};



void UW::ObjectManager::addScriptObjectScript(const std::string &object_name, const std::string &path){
  for (auto& obj : script_objects) {
    if (obj.game_object_data.name == object_name) {
      obj.scripts.emplace_back(path);
      return;
    };
  };
  Engine::Utils::Logger::get().erro("ObjectManager", "Could not find object: " + object_name);
};



void UW::ObjectManager::removeScriptObjectScript(const std::string &object_name, const std::string &path) {
  for (auto& obj : script_objects) {
    if (obj.game_object_data.name == object_name) {
      obj.scripts.erase(
        std::remove_if(obj.scripts.begin(), obj.scripts.end(), 
          [&](const GameObjectScriptRecord& record) {
            return record.getPath() == path;
          }), 
        obj.scripts.end()
      );
      return;
    };
  };
  Engine::Utils::Logger::get().erro("ObjectManager", "Could not find object: " + object_name);
};



void UW::ObjectManager::saveRuntimeObjectScript(const std::string& object_name){
  for (auto& obj : script_objects) {
    if (obj.game_object_data.name == object_name) {
      obj.game_object_data = obj.copy_game_object_data;
    };
  };
};