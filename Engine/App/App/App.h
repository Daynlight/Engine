// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include "Renderer.h"
#include "Core.h"

#include <vector>
#include <functional>

#ifndef PRODUCTION
#include "Editor.h"
#endif

#include "Utils/config.h"
#include "Utils/Logger.h"
#include "Resources/Resources.h"
#include "ScriptShared/GlobResource.h"
#include "Scene.h"



namespace UW{
class App{
private:
  Core core;
  
#ifndef PRODUCTION
  UW::Editor editor;
  float fps = 0.0f;
  float fps_acc = 0.0f;
  unsigned int fps_id = 0;

  float total_fps_acc = 0.0f;
  unsigned int total_fps_id = 0;
#endif

  float fixed_update_time_acc = 0.0f;

public:
  App();
  ~App();

  bool isRunning();
  void run();
  
private:
  // core operations
  void onLoad();
  void onDestroy();
  void render();
  void update();
  void fixedUpdate();
  
#ifndef PRODUCTION
  void updateFps();
#endif

};
};
