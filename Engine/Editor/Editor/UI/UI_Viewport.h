// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once

#ifndef PRODUCTION
#include "Renderer.h"
#include "Gui.h"

#include <functional>

#include "Utils/Logger.h"

#include "UI/Settings.h"



namespace UW{
class UI_Viewport{
private:
  CW::Gui::Gui& gui;
  CW::Renderer::Framebuffer& viewport_fbo;

public:
  UI_Viewport(CW::Gui::Gui& gui, CW::Renderer::Framebuffer& viewport_fbo);
  ~UI_Viewport();
  void uiControl();

private:
  void guiViewport();
  std::function<void(CW::Renderer::iRenderer *window)> ui();

};
};

#endif
