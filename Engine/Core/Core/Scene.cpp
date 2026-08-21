// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "Scene.h"



Engine::Core::Scene::Scene(CW::Renderer::Renderer& window)
  : window(window), camera_controller(&window)
#ifndef PRODUCTION
  , debug_camera(&window)
#endif
{
  Engine::Utils::Logger::get().info("Scene", "Scene Initialized");
};



Engine::Core::Scene::~Scene(){
  Engine::Utils::Logger::get().info("Scene", "Scene Destroyed");
};



void Engine::Core::Scene::onLoad(){
  Engine::Utils::Logger::get().info("Scene", "Loading Scene");
  
  Engine::Utils::Logger::get().info("Scene", "Data Loaded from DataSerializer");

  camera_controller.spawnCamera(
    "Main", 
    {0.0f, 0.0f, 40.0f}, 
    {0.0f, 0.0f, -1.0f}
  );
  camera_controller.setActiveCamera("Main");
  
  Engine::Utils::Logger::get().info("Scene", "Main Camera Initialized");


#ifndef PRODUCTION
  debug_camera.setPosition({0.0f, 0.0f, 0.0f});
  debug_camera.setDirection({0.0f, 0.0f, -1.0f});
  debug_camera.setDefaultMovement(true);
  Engine::Utils::Logger::get().info("Scene", "Debug Camera Initialized");
#endif
};



CW::Renderer::Framebuffer &Engine::Core::Scene::getFbo(){
#ifndef PRODUCTION
  if(debug_camera_on) return debug_camera.getFbo();
  else
#endif
  return camera_controller.getCoreActiveCamera().getFbo();
};



void Engine::Core::Scene::onUpdate(float delta_time){
#ifndef PRODUCTION
  if(debug_camera_on) debug_camera.event(delta_time);
  else
#endif
  camera_controller.getActiveCamera().event(delta_time);

  unsigned int size = Engine::ObjectManager::get().objects.size();
  for(int i = 0; i < size; i++){
    Engine::ObjectManager::get().objects[i].onUpdate(delta_time);
    if(size > Engine::ObjectManager::get().objects.size()){
      size = Engine::ObjectManager::get().objects.size();
      i--;
      if(size == 0) break;
    };
  };

  size = Engine::ObjectManager::get().script_objects.size();
  for(int i = 0; i < size; i++){
    Engine::ObjectManager::get().script_objects[i].onUpdate(delta_time);
    if(size > Engine::ObjectManager::get().script_objects.size()){
      size = Engine::ObjectManager::get().script_objects.size();
      i--;
      if(size == 0) break;
    };
  };
};



void Engine::Core::Scene::onFixedUpdate(float fixed_delta_time){
#ifndef PRODUCTION
  save_acc += fixed_delta_time;

  if(save_acc >= Engine::Config::SAVE_TIMESTAMP){
    save_acc -= Engine::Config::SAVE_TIMESTAMP;
    DataSerializer::get().saveAll();
    Engine::Utils::Logger::get().info("Scene", "Auto-Save scene data");
  };
#endif
  
  unsigned int size = Engine::ObjectManager::get().objects.size();
  for(int i = 0; i < size; i++){
    Engine::ObjectManager::get().objects[i].onFixedUpdate(fixed_delta_time, (*this));
    if(size > Engine::ObjectManager::get().objects.size()){
      size = Engine::ObjectManager::get().objects.size();
      i--;
      if(size == 0) break;
    };
  }; 

  size = Engine::ObjectManager::get().script_objects.size();
  for(int i = 0; i < size; i++){
    Engine::ObjectManager::get().script_objects[i].onFixedUpdate(fixed_delta_time, (*this));
    if(size > Engine::ObjectManager::get().script_objects.size()){
      size = Engine::ObjectManager::get().script_objects.size();
      i--;
      if(size == 0) break;
    };
  };
};



void Engine::Core::Scene::onDestroy() {
  Engine::Utils::Logger::get().info("Scene", "Destroying Scene");

  unsigned int size = Engine::ObjectManager::get().objects.size();
  for(int i = 0; i < size; i++){
    Engine::ObjectManager::get().objects[i].onDestroy();
    if(size > Engine::ObjectManager::get().objects.size()){
      size = Engine::ObjectManager::get().objects.size();
      i--;
      if(size == 0) break;
    };
  };

  size = Engine::ObjectManager::get().script_objects.size();
  for(int i = 0; i < size; i++){
    Engine::ObjectManager::get().script_objects[i].onDestroy();
    if(size > Engine::ObjectManager::get().script_objects.size()){
      size = Engine::ObjectManager::get().script_objects.size();
      i--;
      if(size == 0) break;
    };
  };

  Engine::Utils::Logger::get().info("Scene", "Objects onDestroy");
  
  Engine::ObjectManager::get().objects.clear();
  Engine::ObjectManager::get().script_objects.clear();
  Engine::Utils::Logger::get().info("Scene", "Objects Removed");

  Engine::Utils::Logger::get().info("Scene", "Destroyed Scene");
};



void Engine::Core::Scene::render(){
  Engine::Core::Resources::get().lights.bind(0);
  Engine::Core::Resources::get().materials.bind(1);

#ifndef PRODUCTION
  if(debug_camera_on)
    renderFrame(debug_camera);
  else
#endif
    renderFrame(this->camera_controller.getCoreActiveCamera());

  Engine::Core::Resources::get().materials.unbind();
  Engine::Core::Resources::get().lights.unbind();
};



void Engine::Core::Scene::renderFrame(Engine::Core::Camera& camera){
  camera.clearFbo();
  camera.render(Engine::ObjectManager::get().script_objects);
  camera.render(Engine::ObjectManager::get().objects);
};
