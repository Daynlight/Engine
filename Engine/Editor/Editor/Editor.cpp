// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "Editor.h"



Engine::Editor::Editor::Editor(Engine::Core::Core& core, float& fps, CW::Renderer::Framebuffer& viewport_fbo)
  :core(core), ui(core.window, fps, core.scene, viewport_fbo)
{
  Engine::Utils::Logger::get().info("Editor", "Editor Initialized");
};



Engine::Editor::Editor::~Editor(){
  Engine::Utils::Logger::get().info("Editor", "Editor Destroyed");
};



// ===================================== //
// ========== Editor Operations ========== //
// ===================================== //
void Engine::Editor::Editor::onLoad(){
  Engine::Utils::Logger::get().info("Editor", "Editor Loading");
  ui.onLoad();
  Engine::Utils::Logger::get().info("Editor", "Editor Loaded");
};



void Engine::Editor::Editor::onDestroy() {
  Engine::Utils::Logger::get().info("Editor", "Destroying Editor");
  ui.onDestroy();
  Engine::Utils::Logger::get().info("Editor", "Editor Destroyed");
};



void Engine::Editor::Editor::render(){
  ui.render();
};
