// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "UI_Viewport.h"

#ifndef PRODUCTION



UW::UI_Viewport::UI_Viewport(CW::Gui::Gui &gui, CW::Renderer::Framebuffer& viewport_fbo)
  :viewport_fbo(viewport_fbo), gui(gui){};



UW::UI_Viewport::~UI_Viewport(){
};



void UW::UI_Viewport::uiControl(){
  if(guiSettings.viewportWindowOn){
    Engine::Utils::Logger::get().info("UI", "Opening Viewport Gui");
    gui.addWindow("Viewport Gui", ui());
  }
  else{
    Engine::Utils::Logger::get().info("UI", "Closing Viewport Gui");
    gui.deleteWindow("Viewport Gui");
  };
};



void UW::UI_Viewport::guiViewport() {
  uint32_t textureID = viewport_fbo.getColorTexture(); 
  ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
  
  ImGui::Image(
    reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), 
    viewportPanelSize, 
    ImVec2{ 0.0f, 1.0f }, // uv0
    ImVec2{ 1.0f, 0.0f }  // uv1
  );
};



std::function<void(CW::Renderer::iRenderer *window)> UW::UI_Viewport::ui(){
  return [this](CW::Renderer::iRenderer *window){
    guiViewport();
  };
};

#endif
