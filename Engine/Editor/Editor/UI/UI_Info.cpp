// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "UI_Info.h"

#ifndef PRODUCTION



Engine::Editor::UI_Info::UI_Info(CW::Gui::Gui& gui, float &fps, Engine::Core::Scene& scene)
  :gui(gui), fps(fps), scene(scene){};



Engine::Editor::UI_Info::~UI_Info(){};



void Engine::Editor::UI_Info::uiControl(){
  if(Engine::Editor::guiSettings.infoWindowOn){
    Engine::Utils::Logger::get().info("UI", "Opening Info Gui");
    gui.addWindow("Info Gui", ui());
  }
  else{
    Engine::Utils::Logger::get().info("UI", "Closing Info GUI");
    gui.deleteWindow("Info Gui");  
  };
};



inline void Engine::Editor::UI_Info::guiInfo(){
  ImGui::SeparatorText("Info");
  ImGui::Text("FPS: %f", fps);

  ImGui::Text("Current camera: %s", scene.debug_camera_on ? "Debug" : "Normal");

  ImGui::Text("Camera:");
  ImGui::Text("Name: %s", scene.camera_controller.getActiveCameraName().c_str());
  ImGui::InputFloat3("Camera POS: [%f, %f, %f]", &scene.camera_controller.getActiveCamera().getPosition()[0]);
  ImGui::SliderFloat3("Camera DIR: [%f, %f, %f]", &scene.camera_controller.getActiveCamera().getDirection()[0], -1, 1);

  ImGui::Text("Debug Camera:");
  glm::vec3 debug_cam_pos = scene.debug_camera.getPosition();
  glm::vec3 debug_cam_dir = scene.debug_camera.getDirection();

  ImGui::InputFloat3("Debug POS: [%f, %f, %f]", &debug_cam_pos[0]);
  ImGui::SliderFloat3("Debug DIR: [%f, %f, %f]", &debug_cam_dir[0], -1, 1);
  
  scene.debug_camera.setPosition(debug_cam_pos);
  scene.debug_camera.setDirection(debug_cam_dir);

  if(ImGui::Checkbox("Mesh mode", &Engine::Editor::guiSettings.mesh_mode_on)) mesh_mode_is_updated = false;
  if(!mesh_mode_is_updated){
    mesh_mode_is_updated = true;
    if(Engine::Editor::guiSettings.mesh_mode_on){
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      Engine::Utils::Logger::get().info("UI", "Changed Draw Mode To Mesh");
    }
    else{
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      Engine::Utils::Logger::get().info("UI", "Changed Draw Mode To Normal");
    };
  };

  if(ImGui::Checkbox("Post Processing", &scene.post_processing_on));
  if(ImGui::Checkbox("Shadows", &scene.shadows_on));
};



void Engine::Editor::UI_Info::guiControlsInfo(){
  ImGui::SeparatorText("Controls Info");

  ImGui::Text("- Swap Camera: %s", Engine::Config::SWAP_CAMERA_BTN.c_str());
  ImGui::Text("- Swap Camera Mode: %s", Engine::Config::CAMERA_SWAP_MODE_BTN.c_str());
  ImGui::Text("- Camera Accelerate: %s", Engine::Config::CAMERA_ACCELERATE.c_str());
  ImGui::Text("- Camera Decelerate: %s", Engine::Config::CAMERA_DECELERATE.c_str());
  ImGui::Text("- Move Forward: %s", Engine::Config::CAMERA_MOVE_FORWARD.c_str());
  ImGui::Text("- Move Back: %s", Engine::Config::CAMERA_MOVE_BACK.c_str());
  ImGui::Text("- Move Right: %s", Engine::Config::CAMERA_MOVE_RIGHT.c_str());
  ImGui::Text("- Move Left: %s", Engine::Config::CAMERA_MOVE_LEFT.c_str());
};



inline std::function<void(CW::Renderer::iRenderer *window)> Engine::Editor::UI_Info::ui(){
return [this](CW::Renderer::iRenderer *window){
  guiControlsInfo();
  guiInfo();
};
};

#endif
