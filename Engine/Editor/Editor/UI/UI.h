// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once

#ifndef PRODUCTION
#include <memory>
#include <vector>
#include <algorithm>

#include "Gui.h"
#include "imgui.h"
#include "imgui_internal.h"

#include "Utils/config.h"
#include "Utils/Logger.h"
#include "Objects/ObjectManager.h"
#include "DataSerializer/DataSerializer.h"
#include "ScriptShared/GlobResource.h"
#include "Objects/GameObject.h"
#include "Camera/Camera.h"
#include "Scene.h"

#include "UI/Settings.h"
#include "UI/UI_AssetLoader.h"
#include "UI/UI_ShaderEditors.h"
#include "UI/UI_Info.h"
#include "UI/UI_Logs.h"
#include "UI/UI_Materials.h"
#include "UI/UI_Objects.h"
#include "UI/UI_Lights.h"
#include "UI/UI_Shaders.h"
#include "UI/UI_Scripts.h"
#include "UI/UI_ScriptEditor.h"
#include "UI/UI_Viewport.h"



namespace Engine::Editor{
class UI{
private:
  CW::Gui::Gui gui;
  CW::Renderer::Renderer& window;

  Engine::Core::Scene& scene;

  Engine::Editor::UI_AssetLoader asset_loader_ui;
  Engine::Editor::UI_Info info_ui;
  Engine::Editor::UI_Log log_ui;
  Engine::Editor::UI_Materials materials_ui;
  Engine::Editor::UI_Objects objects_ui;
  Engine::Editor::UI_Lights lights_ui;
  Engine::Editor::UI_Shaders shader_ui;
  Engine::Editor::UI_Scripts scripts_ui;
  Engine::Editor::UI_Viewport viewport_ui;

public:
  UI(CW::Renderer::Renderer &window, float &fps, Engine::Core::Scene& scene, CW::Renderer::Framebuffer& viewport_fbo);
  ~UI();
  void onLoad();
  void render();
  void onDestroy();

private:
  // gui
  void uiLoad();
  void configControl();
  void uiControl();
  void menuBarGui();
  void buildProject();
  void runProject();

  std::function<void(std::function<void()> render_windows)> appWorkspace();

};
};

#endif
