// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include "Renderer.h"

#include <vector>
#include <functional>

#include "config.h"
#include "Utils/Logger.h"
#include "Resources/Resources.h"
#include "ScriptShared/GlobResource.h"
#include "Scene.h"



namespace UW{
class Core{
public:
  CW::Renderer::Renderer window;
  UW::Scene scene;

private:
  std::string cached_title = "";
  unsigned int cached_vsync = 0;
  
#ifndef PRODUCTION
  float camera_swap_cooldown_acc = 0.0f;
#endif

  float fixed_update_time_acc = 0.0f;

public:
  Core();
  ~Core();

  bool isRunning();
  
  // core operations
  void onLoad();
  void onDestroy();
  void render();
  void swapFrame();
  void update();
  void fixedUpdate();

private:
  // helpers
  void initWindow();
  void updateTitle();
  void updateVsync();
  
#ifndef PRODUCTION
  void swapCamera();
#endif

};
};
