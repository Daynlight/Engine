// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "Editor.h"



UW::Editor::Editor(Core& core, float& fps)
  :core(core), ui(core.window, fps, core.scene)
{
  Engine::Utils::Logger::get().info("Editor", "Editor Initialized");
};



UW::Editor::~Editor(){
  Engine::Utils::Logger::get().info("Editor", "Editor Destroyed");
};



// ===================================== //
// ========== Editor Operations ========== //
// ===================================== //
void UW::Editor::onLoad(){
  Engine::Utils::Logger::get().info("Editor", "Editor Loading");
  ui.onLoad();
  Engine::Utils::Logger::get().info("Editor", "Editor Loaded");
};



void UW::Editor::onDestroy() {
  Engine::Utils::Logger::get().info("Editor", "Destroying Editor");
  ui.onDestroy();
  Engine::Utils::Logger::get().info("Editor", "Editor Destroyed");
};



void UW::Editor::render(){
  ui.render();
};
