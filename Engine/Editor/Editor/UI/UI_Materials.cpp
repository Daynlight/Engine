// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "UI_Materials.h"

#ifndef PRODUCTION



Engine::Editor::UI_Materials::UI_Materials(CW::Gui::Gui& gui)
  :gui(gui){};



Engine::Editor::UI_Materials::~UI_Materials(){
};



void Engine::Editor::UI_Materials::uiControl(){
  if(Engine::Editor::guiSettings.materialExplorerOn){
    Engine::Utils::Logger::get().info("UI", "Opening Materials Explorer GUI");
    gui.addWindow("Material Explorer", materialExplorerGui());
  }
  else{
    Engine::Utils::Logger::get().info("UI", "Closing Materials Explorer GUI");
    gui.deleteWindow("Material Explorer");
  };
  
  if(Engine::Editor::guiSettings.materialEditorOn){
    Engine::Utils::Logger::get().info("UI", "Opening Materials Editor GUI");
    gui.addWindow("Material Editor", materialEditorGui());
  }
  else{
    Engine::Utils::Logger::get().info("UI", "Closing Materials Editor GUI");
    gui.deleteWindow("Material Editor");
  };
};



inline void Engine::Editor::UI_Materials::guiMaterialList(){
  ImGui::SeparatorText("Materials List");

  for (std::pair<std::string, Engine::Core::Material> el : Engine::Core::Resources::get().materials.getMaterialReg()) {
    std::string button_label = "- " + el.first;
    if (ImGui::Button(button_label.c_str())) Engine::Editor::guiSettings.material_name = el.first;

    button_label = "Delete ##" + el.first;
    ImGui::SameLine();
    if (ImGui::Button(button_label.c_str())) {
      Engine::Core::Resources::get().materials.erase(el.first);
      Engine::Utils::Logger::get().warn("UI", "Deleted Material { " + el.first + " }");
      break;
    };
  };

  std::string button_label = "Add " + std::to_string(Engine::Core::Resources::get().materials.size());
  if (ImGui::Button(button_label.c_str())) {
    Engine::Core::Resources::get().materials.emplace_back("new material", Engine::Core::Material());
    Engine::Utils::Logger::get().info("UI", "Added new Material { new material }");
  };
};



inline std::function<void(CW::Renderer::iRenderer *window)> Engine::Editor::UI_Materials::materialExplorerGui(){
return [this](CW::Renderer::iRenderer *window){
  guiMaterialList();
};
};



inline void Engine::Editor::UI_Materials::guiMaterialParameters(){
  ImGui::SeparatorText("Materials Parameters");
  ImGui::Text("Material id: %s", Engine::Editor::guiSettings.material_name.c_str());

  if(!Engine::Core::Resources::get().materials.find(guiSettings.material_name)) return;

  Engine::Core::Material temp_mat = Engine::Core::Resources::get().materials.getMaterial(guiSettings.material_name);

  char name_buffer[Engine::Config::OBJECT_NAME_BUFFER_SIZE];
  memcpy(name_buffer, guiSettings.material_name.data(), guiSettings.material_name.size());
  name_buffer[guiSettings.material_name.size()] = '\0';
  if(ImGui::InputText("name", name_buffer, Engine::Config::OBJECT_NAME_BUFFER_SIZE)){
    Engine::Core::Resources::get().materials.erase(guiSettings.material_name);
    guiSettings.material_name = std::string(name_buffer + '\0');
    Engine::Core::Resources::get().materials.emplace_back(guiSettings.material_name, temp_mat);
  };
  
  if(ImGui::ColorEdit3("Albedo: ", &temp_mat.albedo[0])) material_is_updated = true;
  if(ImGui::SliderFloat("Roughness: ", &temp_mat.roughness, 0.0f, 1.0f)) material_is_updated = true;
  if(ImGui::SliderFloat("Metallic: ", &temp_mat.metallic, 0.0f, 1.0f)) material_is_updated = true;
  if(ImGui::ColorEdit3("Emission Color: ", &temp_mat.emission_color[0])) material_is_updated = true;
  if(ImGui::SliderFloat("Emission Strength: ", &temp_mat.emission_strength, 0.0f, 1.0f)) material_is_updated = true;
  if(ImGui::SliderFloat("Ambient Occlusion: ", &temp_mat.ambient_occlusion, 0.0f, 1.0f)) material_is_updated = true;

  if(material_is_updated){
    Engine::Utils::Logger::get().info("UI", "Updating Material { " +  guiSettings.material_name + " }");
    material_is_updated = false;
    Engine::Core::Resources::get().materials[guiSettings.material_name] = temp_mat;
    Engine::Core::Resources::get().materials.compile();
  };
};



std::function<void(CW::Renderer::iRenderer *window)> Engine::Editor::UI_Materials::materialEditorGui(){
  return [this](CW::Renderer::iRenderer *window){
    guiMaterialParameters();
  };
};

#endif
