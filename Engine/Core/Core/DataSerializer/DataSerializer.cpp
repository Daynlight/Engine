// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "DataSerializer.h"

#ifdef PRODUCTION
#include <cmrc/cmrc.hpp>
CMRC_DECLARE(GameData);
#endif

#include "Resources/Resources.h"



UW::DataSerializer &UW::DataSerializer::get(){
  static DataSerializer instance;
  return instance;
};



UW::DataSerializer::DataSerializer(){};



#ifndef PRODUCTION
void UW::DataSerializer::saveAllGlobResources() {
  glob_serializer.saveAll();
};
#endif



void UW::DataSerializer::loadAllGlobResources() {
  glob_serializer.loadAll();
};



#ifndef PRODUCTION
void UW::DataSerializer::saveAllObjects(std::vector<GameObject>& objects) {
  objects_serializer.saveAll(objects);
};
#endif



void UW::DataSerializer::loadAllObjects(std::vector<GameObject>& objects) {
  objects_serializer.loadAll(objects);
};



#ifndef PRODUCTION
void UW::DataSerializer::saveAllMaterials(UW::Materials &materials) {
  materials_serializer.saveAll(materials);
};
#endif



void UW::DataSerializer::loadAllMaterials(UW::Materials &materials) {
  materials_serializer.loadAll(materials);
};



#ifndef PRODUCTION
void UW::DataSerializer::saveAllLights(UW::Lights &lights) {
  lights_serializer.saveAll(lights);
};
#endif



void UW::DataSerializer::loadAllLights(UW::Lights &lights) {
  lights_serializer.loadAll(lights);
};



#ifndef PRODUCTION
void UW::DataSerializer::saveMesh(const std::string &name, const CW::Renderer::Mesh& mesh) {
  mesh_serializer.save(name, mesh);
};
#endif



void UW::DataSerializer::loadMesh(const std::string& path_to_mesh, Engine::Utils::ResourceController<CW::Renderer::Mesh> &meshes) {
  mesh_serializer.load(path_to_mesh, meshes);
};



#ifndef PRODUCTION
void UW::DataSerializer::saveAllMeshes(Engine::Utils::ResourceController<CW::Renderer::Mesh> &meshes) {
  mesh_serializer.saveAll(meshes);
};
#endif



void UW::DataSerializer::loadAllMeshes(Engine::Utils::ResourceController<CW::Renderer::Mesh> &meshes) {
  mesh_serializer.loadAll(meshes);
};



#ifndef PRODUCTION
void UW::DataSerializer::saveShaders(const std::string &shader_name, GLuint type){
  std::string source = Resources::get().getShader(shader_name).getRegisterShader().at(type).getSource();
  shader_serializer.save(shader_name, type, source, Resources::get().shaders);
};
#endif



void UW::DataSerializer::loadShader(const std::string& shader_name){
  shader_serializer.load(shader_name, Resources::get().shaders);
};



#ifndef PRODUCTION
void UW::DataSerializer::saveScript(const std::string &script_name, const std::string& source){
  script_serializer.save(script_name, source);
};
#endif



std::string UW::DataSerializer::loadScript(const std::string& script_name){
  #ifndef PRODUCTION
  return script_serializer.load(script_name);
  #endif
};



void UW::DataSerializer::loadTexture(const std::string &texture_name){
  return texture_serializer.load(texture_name, Resources::get().textures);
};



void UW::DataSerializer::backupGameData() {
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



#ifndef PRODUCTION
void UW::DataSerializer::saveAll() {
  Engine::Utils::Logger::get().info("DataSerializer", "Saving all game data...");
  glob_serializer.saveAll();
  objects_serializer.saveAll(ObjectManager::get().objects);
  materials_serializer.saveAll(Resources::get().materials);
  lights_serializer.saveAll(Resources::get().lights);
  mesh_serializer.saveAll(Resources::get().meshes);
  Engine::Utils::Logger::get().info("DataSerializer", "All game data has been saved");
};
#endif



void UW::DataSerializer::loadAll() {
#ifndef PRODUCTION
  Engine::Utils::Logger::get().info("DataSerializer", "Making Backup...");
  backupGameData();
  Engine::Utils::Logger::get().info("DataSerializer", "Backup done");
#endif

  Engine::Utils::Logger::get().info("DataSerializer", "Loading all game data...");
  glob_serializer.loadAll();
  mesh_serializer.loadAll(Resources::get().meshes);
  lights_serializer.loadAll(Resources::get().lights);
  materials_serializer.loadAll(Resources::get().materials);
  objects_serializer.loadAll(ObjectManager::get().objects);
  shader_serializer.loadAll(Resources::get().shaders);
  texture_serializer.loadAll(Resources::get().textures);
  Engine::Utils::Logger::get().info("DataSerializer", "All game data has been loaded");
};
