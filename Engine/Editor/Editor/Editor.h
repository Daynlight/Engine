// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include "Renderer.h"
#include "Core.h"

#include <vector>
#include <functional>
#include "UI/UI.h"

#include "Utils/config.h"
#include "Utils/Logger.h"
#include "Resources/Resources.h"
#include "ScriptShared/GlobResource.h"
#include "Scene.h"



namespace UW{
class Editor{
private:
  Core& core;
  UI ui;

public:
  Editor(Core& core, float& fps);
  ~Editor();
  
  // Editor operations
  void onLoad();
  void onDestroy();
  void render();

};
};
