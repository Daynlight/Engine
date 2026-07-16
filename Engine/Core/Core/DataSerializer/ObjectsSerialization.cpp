// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "ObjectsSerialization.h"

#ifdef PRODUCTION
#include <cmrc/cmrc.hpp>
CMRC_DECLARE(GameData);
#endif

#include "Objects/Object.h"
#include "Objects/GameObject.h"



#ifndef PRODUCTION
void UW::ObjectsSerialization::save(const UW::GameObject& object) {
  Engine::Utils::Logger::get().info("ObjectsSerialization", "Saving object: " + object.game_object_data.name);
  
  try {
    std::filesystem::path p(Engine::Config::GAME_DATA_FOLDER + Engine::Config::OBJECTS_FILENAME);
    if (p.has_parent_path())
      std::filesystem::create_directories(p.parent_path());
  } catch (const std::filesystem::filesystem_error& e) {
    Engine::Utils::Logger::get().erro("ObjectsSerialization", "Filesystem error - " + std::string(e.what()));
    return;
  };

  std::ofstream outFile(Engine::Config::GAME_DATA_FOLDER + Engine::Config::OBJECTS_FILENAME, std::ios::binary | std::ios::app);
  if (!outFile.is_open()) {
    Engine::Utils::Logger::get().erro("ObjectsSerialization", "Failed to open file for saving");
    return;
  };

  UW::GameObjectRecord record;
  record.name = object.game_object_data.name;
  record.mesh = object.game_object_data.mesh;
  record.shader = object.game_object_data.shader;
  record.position = object.game_object_data.position;
  record.rotation = object.game_object_data.rotation;
  record.scale = object.game_object_data.scale;
  record.textures = object.game_object_data.textures;
  record.materials = object.game_object_data.materials;
  record.parameters = object.game_object_data.parameters;
  record.uniforms = object.game_object_data.uniforms;
  record.culling_on = object.game_object_data.culling_on;
  record.dont_write_to_depth_mask = object.game_object_data.dont_write_to_depth_mask;
  record.gl_depth_lequal = object.game_object_data.gl_depth_lequal;
  record.gl_draw_patches = object.game_object_data.gl_draw_patches;
  record.gl_blend = object.game_object_data.gl_blend;
  for(auto script : object.scripts) record.scripts.emplace_back(std::pair<std::string, bool>(script.getPath(), script.script_on));

  outFile << record;
  outFile.close();

  Engine::Utils::Logger::get().info("ObjectsSerialization", "Object saved { " + object.game_object_data.name + " }");
};
#endif



void UW::ObjectsSerialization::load(UW::GameObject& object) {

};



#ifndef PRODUCTION
void UW::ObjectsSerialization::saveAll(std::vector<UW::GameObject>& objects) {
  Engine::Utils::Logger::get().info("ObjectsSerialization", "Saving all objects...");
  try {
    std::filesystem::path p(Engine::Config::GAME_DATA_FOLDER + Engine::Config::OBJECTS_FILENAME);
    if (p.has_parent_path())
      std::filesystem::create_directories(p.parent_path());
  } catch (const std::filesystem::filesystem_error& e) {
    Engine::Utils::Logger::get().erro("ObjectsSerialization", "Filesystem error - " + std::string(e.what()));
    return;
  }

  std::ofstream outFile(Engine::Config::GAME_DATA_FOLDER + Engine::Config::OBJECTS_FILENAME, std::ios::binary);
  if (!outFile.is_open()) {
    Engine::Utils::Logger::get().erro("ObjectsSerialization", "Failed to open file for saving");
    return;
  };

  size_t obj_size = objects.size();
  outFile.write(reinterpret_cast<const char*>(&obj_size), sizeof(obj_size));

  for (const auto& object : objects) {
    UW::GameObjectRecord record;
    record.name = object.game_object_data.name;
    record.mesh = object.game_object_data.mesh;
    record.shader = object.game_object_data.shader;
    record.position = object.game_object_data.position;
    record.rotation = object.game_object_data.rotation;
    record.scale = object.game_object_data.scale;
    record.textures = object.game_object_data.textures;
    record.materials = object.game_object_data.materials;
    record.parameters = object.game_object_data.parameters;
    record.uniforms = object.game_object_data.uniforms;
    record.culling_on = object.game_object_data.culling_on;
    record.dont_write_to_depth_mask = object.game_object_data.dont_write_to_depth_mask;
    record.gl_depth_lequal = object.game_object_data.gl_depth_lequal;
    record.gl_draw_patches = object.game_object_data.gl_draw_patches;
    record.gl_blend = object.game_object_data.gl_blend;
    for(auto script : object.scripts) record.scripts.emplace_back(std::pair<std::string, bool>(script.getPath(), script.script_on));

    outFile << record;
    Engine::Utils::Logger::get().info("ObjectsSerialization", "Object saved { " + object.game_object_data.name + " }");
  };

  outFile.close();
  Engine::Utils::Logger::get().info("ObjectsSerialization", "All Objects Had Been Saved");
};
#endif



void UW::ObjectsSerialization::loadAll(std::vector<UW::GameObject>& objects) {
  Engine::Utils::Logger::get().info("ObjectsSerialization", "Loading all objects...");
  try {
    std::string resourcePath = Engine::Config::GAME_DATA_FOLDER + Engine::Config::OBJECTS_FILENAME;

#ifndef PRODUCTION
    std::ifstream inFile(resourcePath, std::ios::binary);
    
    if (!inFile.is_open()) {
      Engine::Utils::Logger::get().erro("ObjectsSerialization", "Failed to open file for loading - " + resourcePath);
      return;
    };
#else
    auto fs = cmrc::GameData::get_filesystem();
    
    if (!fs.exists(resourcePath)) {
      Engine::Utils::Logger::get().erro("ObjectsSerialization", "CMRC - File not found - " + resourcePath);
      return;
    };

    auto embeddedFile = fs.open(resourcePath);
    std::string dataStr(embeddedFile.begin(), embeddedFile.end());
    std::stringstream inFile(dataStr);
#endif

    objects.clear();

    size_t objectCount = 0;
    inFile.read(reinterpret_cast<char*>(&objectCount), sizeof(objectCount));

    for (size_t i = 0; i < objectCount; ++i) {
      UW::GameObjectRecord record;
      if (inFile >> record) {
        GameObject object(record.name, record.mesh, record.shader);
        object.game_object_data.position = record.position;
        object.game_object_data.rotation = record.rotation;
        object.game_object_data.scale = record.scale;
        object.game_object_data.culling_on = record.culling_on;
        object.game_object_data.dont_write_to_depth_mask= record.dont_write_to_depth_mask;
        object.game_object_data.gl_depth_lequal = record.gl_depth_lequal;
        object.game_object_data.gl_draw_patches = record.gl_draw_patches;
        object.game_object_data.gl_blend = record.gl_blend;
        object.game_object_data.textures = std::move(record.textures);
        object.game_object_data.materials = std::move(record.materials);
        object.game_object_data.parameters = std::move(record.parameters);
        object.game_object_data.uniforms = std::move(record.uniforms);

        for(auto& script : record.scripts) {
          object.scripts.emplace_back(script.first);
          object.scripts[object.scripts.size() - 1].script_on = script.second;
        };

        objects.push_back(std::move(object));
        Engine::Utils::Logger::get().info("ObjectsSerialization", "Object loaded { " + record.name + " }");
      } else {
        Engine::Utils::Logger::get().erro("ObjectsSerialization", "File format corrupted at index " + std::to_string(i));
        break;
      };
    };
    Engine::Utils::Logger::get().info("ObjectsSerialization", "All objects have been loaded");
  } catch(const std::exception& e) {
    Engine::Utils::Logger::get().erro("ObjectsSerialization", "Exception - " + std::string(e.what()));
  };
};



#ifndef PRODUCTION
std::ostream& UW::operator<<(std::ostream& os, const UW::GameObjectRecord& record) {
  size_t name_sz = record.name.size();
  os.write(reinterpret_cast<const char*>(&name_sz), sizeof(name_sz));
  if (name_sz > 0) os.write(record.name.data(), name_sz);

  size_t mesh_sz = record.mesh.size();
  os.write(reinterpret_cast<const char*>(&mesh_sz), sizeof(mesh_sz));
  if (mesh_sz > 0) os.write(record.mesh.data(), mesh_sz);

  size_t shader_sz = record.shader.size();
  os.write(reinterpret_cast<const char*>(&shader_sz), sizeof(shader_sz));
  if (shader_sz > 0) os.write(record.shader.data(), shader_sz);

  os.write(reinterpret_cast<const char*>(&record.position), sizeof(glm::vec3));
  os.write(reinterpret_cast<const char*>(&record.rotation), sizeof(glm::vec3));
  os.write(reinterpret_cast<const char*>(&record.scale), sizeof(glm::vec3));
  os.write(reinterpret_cast<const char*>(&record.culling_on), sizeof(bool));
  os.write(reinterpret_cast<const char*>(&record.dont_write_to_depth_mask), sizeof(bool));
  os.write(reinterpret_cast<const char*>(&record.gl_depth_lequal), sizeof(bool));
  os.write(reinterpret_cast<const char*>(&record.gl_draw_patches), sizeof(bool));
  os.write(reinterpret_cast<const char*>(&record.gl_blend), sizeof(bool));

  size_t tex_count = record.textures.size();
  os.write(reinterpret_cast<const char*>(&tex_count), sizeof(tex_count));
  for (const auto& tex : record.textures) {
    size_t tex_sz = tex.size();
    os.write(reinterpret_cast<const char*>(&tex_sz), sizeof(tex_sz));
    if (tex_sz > 0) os.write(tex.data(), tex_sz);
  };

  size_t mat_count = record.materials.size();
  os.write(reinterpret_cast<const char*>(&mat_count), sizeof(mat_count));
  for (const auto& mat : record.materials) {
    size_t mat_sz = mat.size();
    os.write(reinterpret_cast<const char*>(&mat_sz), sizeof(mat_sz));
    if (mat_sz > 0) os.write(mat.data(), mat_sz);
  };

  size_t scr_count = record.scripts.size();
  os.write(reinterpret_cast<const char*>(&scr_count), sizeof(scr_count));
  for (const auto& scr : record.scripts) {
    size_t scr_sz = scr.first.size();
    os.write(reinterpret_cast<const char*>(&scr_sz), sizeof(scr_sz));

    if (scr_sz > 0) os.write(scr.first.data(), scr_sz);
    
    os.write(reinterpret_cast<const char*>(&scr.second), sizeof(scr.second));
  };


  size_t param_count = record.parameters.size();
  os.write(reinterpret_cast<const char*>(&param_count), sizeof(param_count));

  for (const auto& [param_name, param_var] : record.parameters) {
    size_t name_sz = param_name.size();
    os.write(reinterpret_cast<const char*>(&name_sz), sizeof(name_sz));
    if (name_sz > 0) os.write(param_name.data(), name_sz);

    size_t type_idx = param_var.index();
    os.write(reinterpret_cast<const char*>(&type_idx), sizeof(type_idx));

    std::visit([&os](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      
      if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, bool>) {
        os.write(reinterpret_cast<const char*>(&arg), sizeof(T));
      }
      else if constexpr (std::is_same_v<T, glm::vec2> || std::is_same_v<T, glm::vec3>) {
        os.write(reinterpret_cast<const char*>(&arg), sizeof(T));
      }
      else if constexpr (std::is_same_v<T, std::string>) {
        size_t str_sz = arg.size();
        os.write(reinterpret_cast<const char*>(&str_sz), sizeof(str_sz));
        if (str_sz > 0) os.write(arg.data(), str_sz);
      }
    }, param_var);
  };


  size_t uni_count = record.uniforms.size();
  os.write(reinterpret_cast<const char*>(&uni_count), sizeof(uni_count));

  for (const auto& [uni_name, uni_var] : record.uniforms) {
    size_t name_sz = uni_name.size();
    os.write(reinterpret_cast<const char*>(&name_sz), sizeof(name_sz));
    if (name_sz > 0) os.write(uni_name.data(), name_sz);

    size_t type_idx = uni_var.index();
    os.write(reinterpret_cast<const char*>(&type_idx), sizeof(type_idx));

    std::visit([&os](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      
      if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, bool>) {
        os.write(reinterpret_cast<const char*>(&arg), sizeof(T));
      }
      else if constexpr (std::is_same_v<T, glm::vec2> || std::is_same_v<T, glm::vec3>) {
        os.write(reinterpret_cast<const char*>(&arg), sizeof(T));
      }
      else if constexpr (std::is_same_v<T, std::string>) {
        size_t str_sz = arg.size();
        os.write(reinterpret_cast<const char*>(&str_sz), sizeof(str_sz));
        if (str_sz > 0) os.write(arg.data(), str_sz);
      }
    }, uni_var);
  };

  return os;
};
#endif



std::istream& UW::operator>>(std::istream& is, UW::GameObjectRecord& record) {
  size_t name_sz = 0;
  if (!is.read(reinterpret_cast<char*>(&name_sz), sizeof(name_sz))) return is;
  record.name.resize(name_sz);
  if (name_sz > 0) is.read(&record.name[0], name_sz);

  size_t mesh_sz = 0;
  is.read(reinterpret_cast<char*>(&mesh_sz), sizeof(mesh_sz));
  record.mesh.resize(mesh_sz);
  if (mesh_sz > 0) is.read(&record.mesh[0], mesh_sz);

  size_t shader_sz = 0;
  is.read(reinterpret_cast<char*>(&shader_sz), sizeof(shader_sz));
  record.shader.resize(shader_sz);
  if (shader_sz > 0) is.read(&record.shader[0], shader_sz);

  is.read(reinterpret_cast<char*>(&record.position), sizeof(glm::vec3));
  is.read(reinterpret_cast<char*>(&record.rotation), sizeof(glm::vec3));
  is.read(reinterpret_cast<char*>(&record.scale), sizeof(glm::vec3));
  is.read(reinterpret_cast<char*>(&record.culling_on), sizeof(bool));
  is.read(reinterpret_cast<char*>(&record.dont_write_to_depth_mask), sizeof(bool));
  is.read(reinterpret_cast<char*>(&record.gl_depth_lequal), sizeof(bool));
  is.read(reinterpret_cast<char*>(&record.gl_draw_patches), sizeof(bool));
  is.read(reinterpret_cast<char*>(&record.gl_blend), sizeof(bool));

  size_t tex_count = 0;
  is.read(reinterpret_cast<char*>(&tex_count), sizeof(tex_count));
  
  if (tex_count > 10000) { 
    is.setstate(std::ios::failbit);
    return is;
  };

  record.textures.resize(tex_count);
  for (auto& tex : record.textures) {
    size_t tex_sz = 0;
    is.read(reinterpret_cast<char*>(&tex_sz), sizeof(tex_sz));
    tex.resize(tex_sz);
    if (tex_sz > 0) is.read(&tex[0], tex_sz);
  }

  size_t mat_count = 0;
  is.read(reinterpret_cast<char*>(&mat_count), sizeof(mat_count));
  
  if (mat_count > 10000) {
    is.setstate(std::ios::failbit);
    return is;
  };

  record.materials.resize(mat_count);
  for (auto& mat : record.materials) {
    size_t mat_sz = 0;
    is.read(reinterpret_cast<char*>(&mat_sz), sizeof(mat_sz));
    mat.resize(mat_sz);
    if (mat_sz > 0) is.read(&mat[0], mat_sz);
  };

  size_t scr_count = 0;
  is.read(reinterpret_cast<char*>(&scr_count), sizeof(scr_count));
  
  if (scr_count > 10000) {
    is.setstate(std::ios::failbit);
    return is;
  };

  record.scripts.resize(scr_count);
  for (auto& scr : record.scripts) {
    size_t scr_sz = 0;
    is.read(reinterpret_cast<char*>(&scr_sz), sizeof(scr_sz));
    scr.first.resize(scr_sz);
    if (scr_sz > 0) is.read(&scr.first[0], scr_sz);

    is.read(reinterpret_cast<char*>(&scr.second), sizeof(scr.second));
  };


  size_t param_count = 0;
  if (!is.read(reinterpret_cast<char*>(&param_count), sizeof(param_count))) return is;

  if (param_count > 10000) {
    is.setstate(std::ios::failbit);
    return is;
  };

  record.parameters.clear();
  for (size_t i = 0; i < param_count; ++i) {
    size_t name_sz = 0;
    is.read(reinterpret_cast<char*>(&name_sz), sizeof(name_sz));
    std::string param_name;
    param_name.resize(name_sz);
    if (name_sz > 0) is.read(&param_name[0], name_sz);

    size_t type_idx = 0;
    is.read(reinterpret_cast<char*>(&type_idx), sizeof(type_idx));

    UW::GameObjectParameterType param_var;
    switch (type_idx) {
      case 0: { // int
        int val = 0;
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        param_var = val;
        break;
      }
      case 1: { // float
        float val = 0.0f;
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        param_var = val;
        break;
      }
      case 2: { // bool
        bool val = false;
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        param_var = val;
        break;
      }
      case 3: { // glm::vec2
        glm::vec2 val(0.0f);
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        param_var = val;
        break;
      }
      case 4: { // glm::vec3
        glm::vec3 val(0.0f);
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        param_var = val;
        break;
      }
      case 5: { // std::string
        size_t str_sz = 0;
        is.read(reinterpret_cast<char*>(&str_sz), sizeof(str_sz));
        std::string val;
        val.resize(str_sz);
        if (str_sz > 0) is.read(&val[0], str_sz);
        param_var = val;
        break;
      }
      default: {
        is.setstate(std::ios::failbit);
        return is;
      };
    };

    record.parameters[param_name] = std::move(param_var);
  };


  size_t uni_count = 0;
  if (!is.read(reinterpret_cast<char*>(&uni_count), sizeof(uni_count))) return is;

  if (uni_count > 10000) {
    is.setstate(std::ios::failbit);
    return is;
  };

  record.uniforms.clear();
  for (size_t i = 0; i < uni_count; ++i) {
    size_t name_sz = 0;
    is.read(reinterpret_cast<char*>(&name_sz), sizeof(name_sz));
    std::string uni_name;
    uni_name.resize(name_sz);
    if (name_sz > 0) is.read(&uni_name[0], name_sz);

    size_t type_idx = 0;
    is.read(reinterpret_cast<char*>(&type_idx), sizeof(type_idx));

    UW::GameObjectParameterType uni_var;
    switch (type_idx) {
      case 0: { // int
        int val = 0;
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        uni_var = val;
        break;
      }
      case 1: { // float
        float val = 0.0f;
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        uni_var = val;
        break;
      }
      case 2: { // bool
        bool val = false;
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        uni_var = val;
        break;
      }
      case 3: { // glm::vec2
        glm::vec2 val(0.0f);
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        uni_var = val;
        break;
      }
      case 4: { // glm::vec3
        glm::vec3 val(0.0f);
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        uni_var = val;
        break;
      }
      case 5: { // std::string
        size_t str_sz = 0;
        is.read(reinterpret_cast<char*>(&str_sz), sizeof(str_sz));
        std::string val;
        val.resize(str_sz);
        if (str_sz > 0) is.read(&val[0], str_sz);
        uni_var = val;
        break;
      }
      default: {
        is.setstate(std::ios::failbit);
        return is;
      };
    };

    record.uniforms[uni_name] = std::move(uni_var);
  };

  return is;
};
