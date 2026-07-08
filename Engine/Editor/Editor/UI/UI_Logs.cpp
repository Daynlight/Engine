// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "UI_Log.h"

#ifndef PRODUCTION



UW::UI_Log::UI_Log(CW::Gui::Gui &gui)
  :gui(gui){};



UW::UI_Log::~UI_Log(){
};



void UW::UI_Log::uiControl(){
  if(guiSettings.logWindowOn){
    Engine::Utils::Logger::get().info("UI", "Opening Log GUI");
    gui.addWindow("Log Gui", ui());
  }
  else{
    Engine::Utils::Logger::get().info("UI", "Closing Log GUI");
    gui.deleteWindow("Log Gui");
  };
};



void UW::UI_Log::guiLogs() {
  const auto& logs = Engine::Utils::Logger::get().getLogs();
  int totalItems = static_cast<int>(logs.size());

  ImGuiListClipper clipper;
  clipper.Begin(totalItems);

  while (clipper.Step()) {
    for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
      const auto& entry = logs[i];
      std::array<float, 4> color = entry.getLogColor();
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color[0], color[1], color[2], color[3]));
      ImGui::TextUnformatted(entry.getText().c_str());
      ImGui::PopStyleColor();
    };
  };

  if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);
};



std::function<void(CW::Renderer::iRenderer *window)> UW::UI_Log::ui(){
  return [this](CW::Renderer::iRenderer *window){
    guiLogs();
  };
};

#endif
