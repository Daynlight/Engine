// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "DataSerializer.h"

#ifdef PRODUCTION
#include <cmrc/cmrc.hpp>
CMRC_DECLARE(GameData);
#endif

#include "Resources/Resources.h"



Engine::DataSerializer &Engine::DataSerializer::get(){
  static DataSerializer instance;
  return instance;
};



Engine::DataSerializer::DataSerializer(){};



#ifndef PRODUCTION
void Engine::DataSerializer::saveAllGlobResources() {
  glob_serializer.saveAll();
};
#endif



void Engine::DataSerializer::loadAllGlobResources() {
  glob_serializer.loadAll();
};



#ifndef PRODUCTION
void Engine::DataSerializer::saveAllObjects(std::vector<GameObject>& objects) {
  objects_serializer.saveAll(objects);
};
#endif



void Engine::DataSerializer::loadAllObjects(std::vector<GameObject>& objects) {
  objects_serializer.loadAll(objects);
};



#ifndef PRODUCTION
void Engine::DataSerializer::saveAllMaterials(Engine::Core::Materials &materials) {
  materials_serializer.saveAll(materials);
};
#endif



void Engine::DataSerializer::loadAllMaterials(Engine::Core::Materials &materials) {
  materials_serializer.loadAll(materials);
};



#ifndef PRODUCTION
void Engine::DataSerializer::saveAllLights(Engine::Core::Lights &lights) {
  lights_serializer.saveAll(lights);
};
#endif



void Engine::DataSerializer::loadAllLights(Engine::Core::Lights &lights) {
  lights_serializer.loadAll(lights);
};



#ifndef PRODUCTION
void Engine::DataSerializer::saveMesh(const std::string &name, const CW::Renderer::Mesh& mesh) {
  mesh_serializer.save(name, mesh);
};
#endif



void Engine::DataSerializer::loadMesh(const std::string& path_to_mesh, Engine::Utils::ResourceController<CW::Renderer::Mesh> &meshes) {
  mesh_serializer.load(path_to_mesh, meshes);
};



#ifndef PRODUCTION
void Engine::DataSerializer::saveAllMeshes(Engine::Utils::ResourceController<CW::Renderer::Mesh> &meshes) {
  mesh_serializer.saveAll(meshes);
};
#endif



void Engine::DataSerializer::loadAllMeshes(Engine::Utils::ResourceController<CW::Renderer::Mesh> &meshes) {
  mesh_serializer.loadAll(meshes);
};



#ifndef PRODUCTION
void Engine::DataSerializer::saveShaders(const std::string &shader_name, GLuint type){
  std::string source = Engine::Core::Resources::get().getShader(shader_name).getRegisterShader().at(type).getSource();
  shader_serializer.save(shader_name, type, source, Engine::Core::Resources::get().shaders);
};
#endif



void Engine::DataSerializer::loadShader(const std::string& shader_name){
  shader_serializer.load(shader_name, Engine::Core::Resources::get().shaders);
};



#ifndef PRODUCTION
void Engine::DataSerializer::saveScript(const std::string &script_name, const std::string& source){
  script_serializer.save(script_name, source);
};
#endif



std::string Engine::DataSerializer::loadScript(const std::string& script_name){
  #ifndef PRODUCTION
  return script_serializer.load(script_name);
  #endif
};



void Engine::DataSerializer::loadTexture(const std::string &texture_name){
  return texture_serializer.load(texture_name, Engine::Core::Resources::get().textures);
};



#ifndef PRODUCTION
void Engine::DataSerializer::backupGameData() {
  Engine::Utils::Logger::get().info("DataSerializer", "Creating backup of GameData...");

  namespace fs = std::filesystem;

  try {
    if (!fs::exists(Engine::Config::GAME_DATA_FOLDER)) {
      Engine::Utils::Logger::get().erro("DataSerializer", "Backup failed: Source folder missing.");
      return;
    };

    if (fs::exists(Engine::Config::BACKUP_GAME_DATA_FOLDER)) fs::remove_all(Engine::Config::BACKUP_GAME_DATA_FOLDER);

    fs::copy(Engine::Config::GAME_DATA_FOLDER, Engine::Config::BACKUP_GAME_DATA_FOLDER, fs::copy_options::recursive | fs::copy_options::overwrite_existing);

    Engine::Utils::Logger::get().info("DataSerializer", "Game data backup completed successfully.");

  } catch (const fs::filesystem_error& e) {
    Engine::Utils::Logger::get().erro("DataSerializer", std::string("Filesystem error during backup: ") + e.what());
  } catch (const std::exception& e) {
    Engine::Utils::Logger::get().erro("DataSerializer", std::string("Unexpected error during backup: ") + e.what());
  };
};



void Engine::DataSerializer::saveAll() {
  Engine::Utils::Logger::get().info("DataSerializer", "Saving all game data...");
  glob_serializer.saveAll();
  objects_serializer.saveAll(ObjectManager::get().objects);
  materials_serializer.saveAll(Engine::Core::Resources::get().materials);
  lights_serializer.saveAll(Engine::Core::Resources::get().lights);
  mesh_serializer.saveAll(Engine::Core::Resources::get().meshes);
  Engine::Utils::Logger::get().info("DataSerializer", "All game data has been saved");
};
#endif



void Engine::DataSerializer::loadAll() {
#ifndef PRODUCTION
  Engine::Utils::Logger::get().info("DataSerializer", "Making Backup...");
  backupGameData();
  Engine::Utils::Logger::get().info("DataSerializer", "Backup done");
#endif

  Engine::Utils::Logger::get().info("DataSerializer", "Loading all game data...");
  glob_serializer.loadAll();
  mesh_serializer.loadAll(Engine::Core::Resources::get().meshes);
  lights_serializer.loadAll(Engine::Core::Resources::get().lights);
  materials_serializer.loadAll(Engine::Core::Resources::get().materials);
  objects_serializer.loadAll(ObjectManager::get().objects);
  shader_serializer.loadAll(Engine::Core::Resources::get().shaders);
  texture_serializer.loadAll(Engine::Core::Resources::get().textures);
  Engine::Utils::Logger::get().info("DataSerializer", "All game data has been loaded");
};
