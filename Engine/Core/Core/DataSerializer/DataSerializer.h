// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include "Renderer.h"

#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <regex>
#include <memory>

#ifdef PRODUCTION
#include <cmrc/cmrc.hpp>
#endif

#include "config.h"
#include "Utils/Logger.h"
#include "Objects/Object.h"
#include "Objects/GameObject.h"
#include "Resources/Materials/Materials.h"

#include "DataSerializer/MeshSerialization.h"
#include "DataSerializer/ObjectsSerialization.h"
#include "DataSerializer/TextureSerialization.h"
#include "DataSerializer/MaterialsSerialization.h"
#include "DataSerializer/LightsSerialization.h"
#include "DataSerializer/ShaderSerialization.h"
#include "DataSerializer/ScriptSerialization.h"
#include "DataSerializer/GlobResourceSerialization.h"



namespace UW {
  class GameObject; 
  class MeshSerialization;
  class ObjectsSerialization;
  class TextureSerialization;
};



namespace UW{
class DataSerializer{
private:
  DataSerializer();
  ~DataSerializer() = default;

  std::unique_ptr<MeshSerialization> mesh_serializer;
  std::unique_ptr<ObjectsSerialization> objects_serializer;
  std::unique_ptr<TextureSerialization> texture_serializer;
  MaterialsSerialization materials_serializer;
  LightsSerialization lights_serializer;
  ShaderSerialization shader_serializer;
  ScriptSerialization script_serializer;
  GlobResourceSerialization glob_serializer;

public:
  static DataSerializer& get();

  DataSerializer(const DataSerializer&) = delete;
  DataSerializer& operator=(const DataSerializer&) = delete;
  DataSerializer(DataSerializer&&) = delete;
  DataSerializer& operator=(DataSerializer&&) = delete;

public:
#ifndef PRODUCTION
  void saveAllGlobResources();
#endif
  void loadAllGlobResources();

#ifndef PRODUCTION
  void saveAllObjects(std::vector<GameObject>& objects);
#endif
  void loadAllObjects(std::vector<GameObject>& objects);

#ifndef PRODUCTION
  void saveAllMaterials(UW::Materials &materials);
#endif
  void loadAllMaterials(UW::Materials &materials);

#ifndef PRODUCTION
  void saveAllLights(UW::Lights &lights);
#endif
  void loadAllLights(UW::Lights &lights);

#ifndef PRODUCTION
  void saveMesh(const std::string& name, const CW::Renderer::Mesh& mesh);
#endif
  void loadMesh(const std::string& path_to_mesh, ResourceController<CW::Renderer::Mesh> &meshes);

#ifndef PRODUCTION
  void saveAllMeshes(ResourceController<CW::Renderer::Mesh> &meshes);
#endif
  void loadAllMeshes(ResourceController<CW::Renderer::Mesh> &meshes);

#ifndef PRODUCTION
  void saveShaders(const std::string& shader_name, GLuint type);
#endif
  void loadShader(const std::string& shader_name);

#ifndef PRODUCTION
  void saveScript(const std::string& script_name, const std::string& source);
#endif
  std::string loadScript(const std::string& script_name);

// #ifndef PRODUCTION
//   void saveTexture(const std::string& script_name, const std::string& source);
// #endif
  void loadTexture(const std::string& texture_name);

#ifndef PRODUCTION
  void backupGameData();
  void saveAll();
#endif

  void loadAll();

};
}; // namespace UW
