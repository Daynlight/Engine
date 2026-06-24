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
  objects.erase(
    std::remove_if(objects.begin(), objects.end(),
      [&name](const UW::GameObject& obj) {
        return obj.game_object_data.name == name;
      }),
    objects.end()
  );
};



UW::GameObjectData *UW::ObjectManager::getGameObjectData(const std::string &name){
  for(auto& object : objects)
    if(object.game_object_data.name == name)
      return &object.game_object_data;

  return nullptr;
};
