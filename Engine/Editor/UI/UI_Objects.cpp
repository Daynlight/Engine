// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "UI_Objects.h"

#ifndef PRODUCTION



UW::UI_Objects::UI_Objects(CW::Gui::Gui& gui, CW::Renderer::Renderer& window, UW::Scene& scene)
  :gui(gui), window(window), scene(scene) {};



UW::UI_Objects::~UI_Objects(){
};



void UW::UI_Objects::uiControl(){
  if(guiSettings.objectExplorerWindowOn){
    Logger::get().info("UI", "Opening Object Explorer GUI");
    gui.addWindow("Object Explorer", objectExplorerGui());
  }
  else{
    Logger::get().info("UI", "Closing Object Explorer GUI");
    gui.deleteWindow("Object Explorer");
  };

  if(guiSettings.objectEditorWindowOn){
    Logger::get().info("UI", "Opening Object Editor GUI");
    gui.addWindow("Object Editor", objectEditorGui());
  }
  else{
    Logger::get().info("UI", "Closing Object Explorer GUI");
    gui.deleteWindow("Object Editor");
  };
};



void UW::UI_Objects::guiObjectList(){
  ImGui::SeparatorText("Object List");

  for(unsigned int id = 0; id < UW::ObjectManager::get().objects.size(); id++){
    if(UW::ObjectManager::get().objects[id].copy_game_object_data.hidden) continue;

    std::string label = "- " + UW::ObjectManager::get().objects[id].game_object_data.name + "##(" + std::to_string(id) + ")";
    if(ImGui::Button(label.c_str())) guiSettings.object_id = id;
    
    label = "Delete##" + std::to_string(id);
    ImGui::SameLine();
    if(ImGui::Button(label.c_str())) {
      UW::ObjectManager::get().objects.erase(UW::ObjectManager::get().objects.begin() + id);
      Logger::get().warn("UI", "Deleted Object { " + UW::ObjectManager::get().objects[id].game_object_data.name + " }");
    };

    label = "Duplicate##" + std::to_string(id);
    ImGui::SameLine();
    if(ImGui::Button(label.c_str())) {
      UW::ObjectManager::get().objects.emplace_back(GameObject(UW::ObjectManager::get().objects[id].game_object_data.name + "_copy", UW::ObjectManager::get().objects[id]));
      Logger::get().warn("UI", "Duplicated Object { " + UW::ObjectManager::get().objects[id].game_object_data.name + " }");
    };
  };

  if(ImGui::Button("Add new")) {
    UW::ObjectManager::get().objects.emplace_back(UW::GameObject("new object", "testing", "testing", {}, {}, {}, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
    Logger::get().info("UI", "Added New Object { new object }");
  };
};



std::function<void(CW::Renderer::iRenderer *window)> UW::UI_Objects::objectExplorerGui(){
return [this](CW::Renderer::iRenderer *window){
  guiObjectList();
};
};



void UW::UI_Objects::guiObjectEditor(){
  ImGui::SeparatorText("Object Editor");
  if(guiSettings.object_id >= UW::ObjectManager::get().objects.size()) return;
  
  UW::GameObject& object = UW::ObjectManager::get().objects[guiSettings.object_id];

  char name_buffer[UW::Config::OBJECT_NAME_BUFFER_SIZE];
  memcpy(name_buffer, object.game_object_data.name.data(), object.game_object_data.name.size());
  name_buffer[object.game_object_data.name.size()] = '\0';
  if(ImGui::InputText("name", name_buffer, UW::Config::OBJECT_NAME_BUFFER_SIZE)){
    object.stopScripts();
    object.game_object_data.name = std::string(name_buffer + '\0');
    object.startScripts();
  };
  
  char mesh_buffer[UW::Config::OBJECT_NAME_BUFFER_SIZE];
  memcpy(mesh_buffer, object.game_object_data.mesh.data(), object.game_object_data.mesh.size());
  mesh_buffer[object.game_object_data.mesh.size()] = '\0';
  if(ImGui::InputText("mesh", mesh_buffer, UW::Config::OBJECT_NAME_BUFFER_SIZE)){
    if(!Resources::get().meshes.exists(mesh_buffer)) return;
    object.stopScripts();
    object.game_object_data.mesh = std::string(mesh_buffer + '\0');
    object.startScripts();
  };

  char shader_buffer[UW::Config::OBJECT_NAME_BUFFER_SIZE];
  memcpy(shader_buffer, object.game_object_data.shader.data(), object.game_object_data.shader.size());
  shader_buffer[object.game_object_data.shader.size()] = '\0';
  if(ImGui::InputText("shader", shader_buffer, UW::Config::OBJECT_NAME_BUFFER_SIZE)){
    auto its = Resources::get().shaders.find(shader_buffer);
    if(its == Resources::get().shaders.end()) return;
    object.stopScripts();
    object.game_object_data.shader = std::string(shader_buffer + '\0');
    object.startScripts();
  };


  glm::vec3 new_position = object.game_object_data.position;
  if(ImGui::InputFloat3("position: ", &new_position[0])) {
    object.stopScripts();
    object.game_object_data.position = new_position;
    object.startScripts();
  };

  glm::vec3 position_offset = glm::vec3(0.0f);
  if(ImGui::SliderFloat3("position slider: ", &position_offset[0], -10.0f, 10.0f)){
    object.stopScripts();
    object.game_object_data.position += position_offset * window.getWindowData()->delta_time;
    object.startScripts();
  };

  glm::vec3 new_rotation = object.game_object_data.rotation;
  if(ImGui::InputFloat3("rotate: ", &new_rotation[0])){
    object.stopScripts();
    object.game_object_data.rotation = new_rotation;
    object.startScripts();
  };

  glm::vec3 rotate_offset = glm::vec3(0.0f);
  if(ImGui::SliderFloat3("rotate slider: ", &rotate_offset[0], -1.0f, 1.0f)){
    object.stopScripts();
    object.game_object_data.rotation += rotate_offset * window.getWindowData()->delta_time;
    object.startScripts();
  };

  glm::vec3 new_scale = object.game_object_data.scale;
  if(ImGui::InputFloat3("scale: ", &new_scale[0])){
    object.stopScripts();
    object.game_object_data.scale = new_scale;
    object.startScripts();
  };

  glm::vec3 scale_offset = glm::vec3(0.0f);
  if(ImGui::SliderFloat3("scale slider: ", &scale_offset[0], -100.0f, 100.0f)){
    object.stopScripts();
    object.game_object_data.scale += scale_offset * window.getWindowData()->delta_time;
    object.startScripts();
  };

  bool culling_on = object.game_object_data.culling_on;
  if(ImGui::Checkbox("Culling", &culling_on)){
    object.stopScripts();
    object.game_object_data.culling_on = culling_on;
    object.startScripts();
  };

  bool dont_write_to_depth_mask = object.game_object_data.dont_write_to_depth_mask;
  if(ImGui::Checkbox("DontWriteToDepth", &dont_write_to_depth_mask)){
    object.stopScripts();
    object.game_object_data.dont_write_to_depth_mask = dont_write_to_depth_mask;
    object.startScripts();
  };

  bool gl_depth_lequal = object.game_object_data.gl_depth_lequal;
  if(ImGui::Checkbox("DepthLEQ", &gl_depth_lequal)){
    object.stopScripts();
    object.game_object_data.gl_depth_lequal = gl_depth_lequal;
    object.startScripts();
  };

  bool gl_draw_patches = object.game_object_data.gl_draw_patches;
  if(ImGui::Checkbox("DrawPatches", &gl_draw_patches)){
    object.stopScripts();
    object.game_object_data.gl_draw_patches = gl_draw_patches;
    object.startScripts();
  };

  bool gl_blend = object.game_object_data.gl_blend;
  if(ImGui::Checkbox("Blend", &gl_blend)){
    object.stopScripts();
    object.game_object_data.gl_blend = gl_blend;
    object.startScripts();
  };


  ImGui::SeparatorText("Textures: ");
  for(int i = 0; i < object.game_object_data.textures.size(); i++){
    std::string label = "- texture (" + std::to_string(i) + ")";
    char texture_buffer[UW::Config::OBJECT_NAME_BUFFER_SIZE];
    memcpy(texture_buffer, object.game_object_data.textures[i].data(), object.game_object_data.textures[i].size());
    texture_buffer[object.game_object_data.textures[i].size()] = '\0';
    if(ImGui::InputText(label.c_str(), texture_buffer, UW::Config::OBJECT_NAME_BUFFER_SIZE)){
      object.stopScripts();
      object.game_object_data.textures[i] = std::string(texture_buffer + '\0');
      object.startScripts();
    };
    
    ImGui::SameLine();
    label = "Delete texture##(" + std::to_string(i) + ")";
    if(ImGui::Button(label.c_str())) {
      object.stopScripts();
      object.game_object_data.textures.erase(object.game_object_data.textures.begin() + i);
      object.startScripts();
    };
  };

  std::string label = "Add Texture (" + std::to_string(object.game_object_data.textures.size()) + ")";
  if(ImGui::Button(label.c_str())) {
    object.stopScripts();
    object.game_object_data.textures.emplace_back("");
    object.startScripts();
  };


  ImGui::SeparatorText("Materials: ");
  for(int i = 0; i < object.game_object_data.materials.size(); i++){
    std::string label = "- material (" + std::to_string(i) + ")";
    
    char material_buffer[UW::Config::OBJECT_NAME_BUFFER_SIZE];
    memcpy(material_buffer, object.game_object_data.materials[i].data(), object.game_object_data.materials[i].size());
    material_buffer[object.game_object_data.materials[i].size()] = '\0';
    if(ImGui::InputText(label.c_str(), material_buffer, UW::Config::OBJECT_NAME_BUFFER_SIZE)){
      object.stopScripts();
      if(!Resources::get().materials.find(material_buffer)) return;
      object.game_object_data.materials[i] = std::string(material_buffer + '\0');
      object.startScripts();
    };
    
    ImGui::SameLine();
    label = "Delete material##(" + std::to_string(i) + ")";
    if(ImGui::Button(label.c_str())) {
      object.stopScripts();
      object.game_object_data.materials.erase(object.game_object_data.materials.begin() + i);
      object.startScripts();
    }
  };

  label = "Add material (" + std::to_string(object.game_object_data.materials.size()) + ")";
  if(ImGui::Button(label.c_str())) {
    object.stopScripts();
    object.game_object_data.materials.emplace_back("new material");
    object.startScripts();
  };


  ImGui::SeparatorText("Scripts: ");
  for(int i = 0; i < object.scripts.size(); i++){
    bool new_script_on = object.scripts[i].script_on;
    if(ImGui::Checkbox(std::string("##ScriptOn(" + std::to_string(i) + ")").c_str(), &new_script_on)){
      object.stopScripts();
      object.scripts[i].script_on = new_script_on;
      object.startScripts();
    };
    
    ImGui::SameLine();
    std::string label = "- script (" + std::to_string(i) + ")";
    
    char script_buffer[UW::Config::OBJECT_NAME_BUFFER_SIZE];
    memcpy(script_buffer, object.scripts[i].getPath().data(), object.scripts[i].getPath().size());
    script_buffer[object.scripts[i].getPath().size()] = '\0';
    if(ImGui::InputText(label.c_str(), script_buffer, UW::Config::OBJECT_NAME_BUFFER_SIZE)){
      object.stopScripts();
      object.scripts[i] = UW::GameObjectScriptRecord(std::string(script_buffer + '\0'));
      object.scripts[i].script_on = new_script_on;
      object.startScripts();
    }
    
    ImGui::SameLine();
    label = "Delete scripts##(" + std::to_string(i) + ")";
    if(ImGui::Button(label.c_str())) {
      object.stopScripts();
      object.scripts.erase(object.scripts.begin() + i);
      object.startScripts();
    };
  };

  label = "Add script (" + std::to_string(object.scripts.size()) + ")";
  if(ImGui::Button(label.c_str())) {
    object.stopScripts();
    object.scripts.emplace_back(GameObjectScriptRecord("new script"));
    object.startScripts();
  };


  ImGui::SeparatorText("Parameters");

  auto& params = object.game_object_data.parameters;

  for (auto it = params.begin(); it != params.end();) {
    const std::string& current_name = it->first;
    auto& param_value = it->second;

    ImGui::PushID(current_name.c_str());

    bool delete_triggered = false;
    bool rename_triggered = false;
    char name_buffer[128];
    strncpy(name_buffer, current_name.c_str(), sizeof(name_buffer) - 1);
    name_buffer[sizeof(name_buffer) - 1] = '\0';

    ImGui::SetNextItemWidth(120.0f);
    if (ImGui::InputText("##ParamName", name_buffer, sizeof(name_buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
      rename_triggered = true;
    }
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      rename_triggered = true;
    }

    ImGui::SameLine();

    int current_type_idx = static_cast<int>(param_value.index());
    ImGui::SetNextItemWidth(70.0f);
    if (ImGui::Combo("##ParamType", &current_type_idx, UW::gameObjectParameterTypeName, IM_ARRAYSIZE(UW::gameObjectParameterTypeName))) {
      object.stopScripts();
      switch (current_type_idx) {
        case 0: param_value = 0; break;
        case 1: param_value = 0.0f; break;
        case 2: param_value = false; break;
        case 3: param_value = glm::vec2(0.0f); break;
        case 4: param_value = glm::vec3(0.0f); break;
        case 5: param_value = std::string(""); break;
      }
      object.startScripts();
      ImGui::PopID();
      break;
    }

    ImGui::SameLine();

    std::visit([&object](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      ImGui::SetNextItemWidth(150.0f);
      
      if constexpr (std::is_same_v<T, int>) {
        int new_arg = arg;
        if(ImGui::InputInt("##val", &new_arg)){
          object.stopScripts();
          arg = new_arg;
          object.startScripts();
        }
      }
      else if constexpr (std::is_same_v<T, float>) {
        float new_arg = arg;
        if(ImGui::DragFloat("##val", &new_arg, 0.05f)){
          object.stopScripts();
          arg = new_arg;
          object.startScripts();
        }
      }
      else if constexpr (std::is_same_v<T, bool>) {
        bool new_arg = arg;
        if(ImGui::Checkbox("##val", &new_arg)){
          object.stopScripts();
          arg = new_arg;
          object.startScripts();
        }
      }
      else if constexpr (std::is_same_v<T, glm::vec2>) {
        glm::vec2 new_arg = arg;
        if(ImGui::DragFloat2("##val", &new_arg.x, 0.05f)){
          object.stopScripts();
          arg = new_arg;
          object.startScripts();
        }
      }
      else if constexpr (std::is_same_v<T, glm::vec3>) {
        glm::vec3 new_arg = arg;
        if(ImGui::DragFloat3("##val", &new_arg.x, 0.05f)){
          object.stopScripts();
          arg = new_arg;
          object.startScripts();
        }
      }
      else if constexpr (std::is_same_v<T, std::string>) {
        char str_buffer[256];
        strncpy(str_buffer, arg.c_str(), sizeof(str_buffer) - 1);
        str_buffer[sizeof(str_buffer) - 1] = '\0';
        if (ImGui::InputText("##val", str_buffer, sizeof(str_buffer))) {
          object.stopScripts();
          arg = std::string(str_buffer);
          object.startScripts();
        }
      }
    }, param_value);

    ImGui::SameLine();

    if (ImGui::Button("Delete")) {
      delete_triggered = true;
    }

    ImGui::PopID();

    if (delete_triggered) {
      object.stopScripts();
      it = params.erase(it);
      object.startScripts();
    } 
    else if (rename_triggered && std::string(name_buffer) != current_name && !std::string(name_buffer).empty()) {
      std::string new_key = name_buffer;
      
      object.stopScripts();
      if (params.find(new_key) == params.end()) {
        params[new_key] = std::move(param_value);
        it = params.erase(it);
      } else {
        ++it;
      }
      object.startScripts();
    } 
    else {
      ++it;
    }
  }

  ImGui::Spacing();

  std::string add_label = "Add Parameter (" + std::to_string(params.size()) + ")";
  if (ImGui::Button(add_label.c_str())) {
    object.stopScripts();
    std::string unique_new_name = "NewParameter_" + std::to_string(params.size());
    
    int safety_counter = 0;
    while(params.find(unique_new_name) != params.end()) {
      unique_new_name = "NewParameter_" + std::to_string(params.size() + (++safety_counter));
    }
    
    params[unique_new_name] = 0;
    object.startScripts();
  }



  ImGui::SeparatorText("Uniforms");

  auto& uniforms = object.game_object_data.uniforms;

  for (auto it = uniforms.begin(); it != uniforms.end();) {
    const std::string& current_name = it->first;
    auto& uniform_value = it->second;

    ImGui::PushID(current_name.c_str());

    bool delete_triggered = false;
    bool rename_triggered = false;
    char name_buffer[128];
    strncpy(name_buffer, current_name.c_str(), sizeof(name_buffer) - 1);
    name_buffer[sizeof(name_buffer) - 1] = '\0';

    ImGui::SetNextItemWidth(120.0f);
    if (ImGui::InputText("##UniformName", name_buffer, sizeof(name_buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
      rename_triggered = true;
    }
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      rename_triggered = true;
    }

    ImGui::SameLine();

    int current_type_idx = static_cast<int>(uniform_value.index());
    ImGui::SetNextItemWidth(70.0f);
    if (ImGui::Combo("##UniformType", &current_type_idx, UW::gameObjectParameterTypeName, IM_ARRAYSIZE(UW::gameObjectParameterTypeName))) {
      object.stopScripts();
      switch (current_type_idx) {
        case 0: uniform_value = 0; break;
        case 1: uniform_value = 0.0f; break;
        case 2: uniform_value = false; break;
        case 3: uniform_value = glm::vec2(0.0f); break;
        case 4: uniform_value = glm::vec3(0.0f); break;
        case 5: uniform_value = std::string(""); break;
      }
      object.startScripts();

      ImGui::PopID();
      break;
    }

    ImGui::SameLine();

    std::visit([&object](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      ImGui::SetNextItemWidth(150.0f);
      
      if constexpr (std::is_same_v<T, int>) {
        int new_arg = arg;
        if(ImGui::InputInt("##val", &new_arg)){
          object.stopScripts();
          arg = new_arg;
          object.startScripts();
        }
      }
      else if constexpr (std::is_same_v<T, float>) {
        float new_arg = arg;
        if(ImGui::DragFloat("##val", &new_arg, 0.05f)){
          object.stopScripts();
          arg = new_arg;
          object.startScripts();
        }
      }
      else if constexpr (std::is_same_v<T, bool>) {
        bool new_arg = arg;
        if(ImGui::Checkbox("##val", &new_arg)){
          object.stopScripts();
          arg = new_arg;
          object.startScripts();
        }
      }
      else if constexpr (std::is_same_v<T, glm::vec2>) {
        glm::vec2 new_arg = arg;
        if(ImGui::DragFloat2("##val", &new_arg.x, 0.05f)){
          object.stopScripts();
          arg = new_arg;
          object.startScripts();
        }
      }
      else if constexpr (std::is_same_v<T, glm::vec3>) {
        glm::vec3 new_arg = arg;
        if(ImGui::DragFloat3("##val", &new_arg.x, 0.05f)){
          object.stopScripts();
          arg = new_arg;
          object.startScripts();
        }
      }
      else if constexpr (std::is_same_v<T, std::string>) {
        char str_buffer[256];
        strncpy(str_buffer, arg.c_str(), sizeof(str_buffer) - 1);
        str_buffer[sizeof(str_buffer) - 1] = '\0';
        if (ImGui::InputText("##val", str_buffer, sizeof(str_buffer))) {
          object.stopScripts();
          arg = std::string(str_buffer);
          object.startScripts();
        }
      }
    }, uniform_value);

    ImGui::SameLine();

    if (ImGui::Button("Delete")) {
      delete_triggered = true;
    }

    ImGui::PopID();

    if (delete_triggered) {
      object.stopScripts();
      it = uniforms.erase(it);
      object.startScripts();
    } 
    else if (rename_triggered && std::string(name_buffer) != current_name && !std::string(name_buffer).empty()) {
      std::string new_key = name_buffer;
      
      object.stopScripts();
      if (uniforms.find(new_key) == uniforms.end()) {
        uniforms[new_key] = std::move(uniform_value);
        it = uniforms.erase(it);
      } else {
        ++it;
      }
      object.startScripts();
    } 
    else {
      ++it;
    }
  }

  ImGui::Spacing();

  std::string uniform_add_label = "Add Uniform (" + std::to_string(uniforms.size()) + ")";
  if (ImGui::Button(uniform_add_label.c_str())) {
    object.stopScripts();
    std::string unique_new_name = "NewUniform_" + std::to_string(uniforms.size());
    
    int safety_counter = 0;
    while(uniforms.find(unique_new_name) != uniforms.end()) {
      unique_new_name = "NewUniform_" + std::to_string(uniforms.size() + (++safety_counter));
    }
    
    uniforms[unique_new_name] = 0;
    object.startScripts();
  }
};



std::function<void(CW::Renderer::iRenderer *window)> UW::UI_Objects::objectEditorGui(){
  return [this](CW::Renderer::iRenderer *window){
    guiObjectEditor();
  };
};

#endif
