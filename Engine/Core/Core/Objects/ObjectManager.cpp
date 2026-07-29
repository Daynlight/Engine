// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "ObjectManager.h"



Engine::ObjectManager &Engine::ObjectManager::get(){
  static ObjectManager instance;
  return instance; 
};



void Engine::ObjectManager::emplace_back(const std::string &name){
  objects.emplace_back(Engine::GameObject(name, "empty", "Default"));
};



void Engine::ObjectManager::erase(const std::string &name) {
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



Engine::ScriptShared::GameObjectData *Engine::ObjectManager::getGameObjectData(const std::string &name){
  for(auto& object : objects)
    if(object.game_object_data.name == name)
      return &object.copy_game_object_data;

  return nullptr;
};



void Engine::ObjectManager::addScript(const std::string &object_name, const std::string &path){
  for (auto& obj : objects) {
    if (obj.game_object_data.name == object_name) {
      obj.scripts.emplace_back(path);
      return;
    };
  };
  Engine::Utils::Logger::get().erro("ObjectManager", "Could not find object: " + object_name);
};



void Engine::ObjectManager::removeScript(const std::string &object_name, const std::string &path) {
  for (auto& obj : objects) {
    if (obj.game_object_data.name == object_name) {
      obj.scripts.erase(
        std::remove_if(obj.scripts.begin(), obj.scripts.end(), 
          [&](const Engine::Core::Script::GameObjectScriptRecord& record) {
            return record.getPath() == path;
          }), 
        obj.scripts.end()
      );
      return;
    };
  };
  Engine::Utils::Logger::get().erro("ObjectManager", "Could not find object: " + object_name);
};



void Engine::ObjectManager::saveRuntime(const std::string& object_name){
  for (auto& obj : objects) {
    if (obj.game_object_data.name == object_name) {
      obj.game_object_data = obj.copy_game_object_data;
    };
  };
};



void Engine::ObjectManager::emplace_backObjectScript(const std::string &name){
  script_objects.emplace_back(Engine::GameObject(name, "empty", "Default"));
};



void Engine::ObjectManager::eraseObjectScript(const std::string &name) {
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



Engine::ScriptShared::GameObjectData *Engine::ObjectManager::getGameObjectDataObjectScript(const std::string &name){
  for(auto& object : script_objects)
    if(object.game_object_data.name == name)
      return &object.copy_game_object_data;

  return nullptr;
};



void Engine::ObjectManager::addScriptObjectScript(const std::string &object_name, const std::string &path){
  for (auto& obj : script_objects) {
    if (obj.game_object_data.name == object_name) {
      obj.scripts.emplace_back(path);
      return;
    };
  };
  Engine::Utils::Logger::get().erro("ObjectManager", "Could not find object: " + object_name);
};



void Engine::ObjectManager::removeScriptObjectScript(const std::string &object_name, const std::string &path) {
  for (auto& obj : script_objects) {
    if (obj.game_object_data.name == object_name) {
      obj.scripts.erase(
        std::remove_if(obj.scripts.begin(), obj.scripts.end(), 
          [&](const Engine::Core::Script::GameObjectScriptRecord& record) {
            return record.getPath() == path;
          }), 
        obj.scripts.end()
      );
      return;
    };
  };
  Engine::Utils::Logger::get().erro("ObjectManager", "Could not find object: " + object_name);
};



void Engine::ObjectManager::saveRuntimeObjectScript(const std::string& object_name){
  for (auto& obj : script_objects) {
    if (obj.game_object_data.name == object_name) {
      obj.game_object_data = obj.copy_game_object_data;
    };
  };
};