// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "UI_ShaderEditors.h"
#ifndef PRODUCTION



Engine::Editor::UI_ShaderEditor::UI_ShaderEditor(CW::Gui::Gui& gui, const std::string& name, GLenum type)
  :gui(gui), shader_name(name), shader_type(type){

  Engine::Utils::Logger::get().info("UI_ShaderEditor", "Opened { " + shader_name + " : " + Engine::Config::SHADER_TYPE_TO_NAME[shader_type] + " }");
  gui.addWindow("Shader Editor " + shader_name + " : " + Engine::Config::SHADER_TYPE_TO_NAME[shader_type], shaderEditorGui());
};



Engine::Editor::UI_ShaderEditor::~UI_ShaderEditor(){
  gui.deleteWindow("Shader Editor " + shader_name + " : " + Engine::Config::SHADER_TYPE_TO_NAME[shader_type]);
  Engine::Utils::Logger::get().info("UI_ShaderEditor", "Closed { " + shader_name + " : " + Engine::Config::SHADER_TYPE_TO_NAME[shader_type] + " }");
};



void Engine::Editor::UI_ShaderEditor::guiShaderLoad(const std::string& name, GLenum type){
  if(shader_is_loaded) return;

  shader_name = name;
  shader_type = type;
  memset(buffer, '\0', Engine::Config::SHADER_EDITOR_BUFFER_SIZE);
  
  auto it = Engine::Core::Resources::get().shaders.find(name);
  if(it == Engine::Core::Resources::get().shaders.end()) return;

  const std::unordered_map<GLenum, CW::Renderer::ShaderData>& reg = Engine::Core::Resources::get().getShader(name).getRegisterShader();
  auto ita = reg.find(type);
  if(ita == reg.end()) return;

  std::string source = reg.at(type).getSource();
  memcpy(buffer, source.data(), source.size());


  Engine::Utils::Logger::get().info("UI_ShaderEditor", "Loaded { " + shader_name + " : " + Engine::Config::SHADER_TYPE_TO_NAME[shader_type] + " }");
  shader_is_loaded = true;
};



void Engine::Editor::UI_ShaderEditor::guiShaderEditor(){
  float width = ImGui::GetContentRegionAvail().x;
  float height = ImGui::GetContentRegionAvail().y - 50.0f;
  
  ImGui::SeparatorText("Shader Editor");
  ImGui::Text("Shader: %s : %s", shader_name.c_str(), Engine::Config::SHADER_TYPE_TO_NAME[shader_type].c_str());
  
  ImGui::InputTextMultiline("##Shader Content", buffer, Engine::Config::SHADER_EDITOR_BUFFER_SIZE, ImVec2(width, height), ImGuiInputTextFlags_WordWrap);

  auto it = Engine::Core::Resources::get().shaders.find(shader_name);
  if(it == Engine::Core::Resources::get().shaders.end()) return;
  
  auto& reg = Engine::Core::Resources::get().getShader(shader_name).getRegisterShader();
  auto it2 = reg.find(shader_type);
  if(it2 == reg.end()) return;

  if(strcmp(buffer, reg.at(shader_type).getSource().c_str()) != 0) shader_is_updated = true;

  if(shader_is_updated){
    shader_is_updated = false;
    
    Engine::Core::Resources::get().getShader(shader_name).destroy();
    Engine::Core::Resources::get().getShader(shader_name).removeShaders(shader_type);
    Engine::Core::Resources::get().getShader(shader_name).setShader(buffer, shader_type);
    Engine::Core::Resources::get().getShader(shader_name).compile();
    DataSerializer::get().saveShaders(shader_name, shader_type);

    Engine::Utils::Logger::get().info("UI_ShaderEditor", "Saved { " + shader_name + " : " + Engine::Config::SHADER_TYPE_TO_NAME[shader_type] + " }");
  };
};



inline std::function<void(CW::Renderer::iRenderer *window)> Engine::Editor::UI_ShaderEditor::shaderEditorGui(){
return [this](CW::Renderer::iRenderer *window){
  guiShaderLoad(shader_name, shader_type);
  guiShaderEditor();
};
};



std::string Engine::Editor::UI_ShaderEditor::getName(){
  return shader_name;
};



GLenum Engine::Editor::UI_ShaderEditor::getType(){
  return shader_type;
};

#endif
