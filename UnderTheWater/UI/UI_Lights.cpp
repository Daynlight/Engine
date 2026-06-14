#include "UI_Lights.h"

#ifndef PRODUCTION



UW::UI_Lights::UI_Lights(CW::Gui::Gui& gui)
  :gui(gui){};



UW::UI_Lights::~UI_Lights(){
};



void UW::UI_Lights::uiControl(){
  if(guiSettings.lightsExplorerOn){
    Logger::get().info("UI", "Opening Lights Explorer GUI");
    gui.addWindow("Lights Editor", ui());
  }
  else{
    Logger::get().info("UI", "Closing Lights Explorer GUI");
    gui.deleteWindow("Lights Editor");
  };
};



void UW::UI_Lights::guiLights(){
  ImGui::SeparatorText("Lights");
  bool lights_updated = false;

  for(auto& lights : Resources::get().lights){
    ImGui::SeparatorText(lights.first.c_str());

    for(unsigned int i = 0; i < lights.second.size(); i++){
      UW::Light& light = lights.second[i];
      std::string label = lights.first + " (" + std::to_string(i) + ")"; 
      ImGui::Text(label.c_str());
      if(ImGui::InputFloat3(("position: [%f, %f, %f] ##" + lights.first + std::to_string(i)).c_str(), &light.position[0])) lights_updated = true;
      if(ImGui::ColorEdit3(("color:  ##" + lights.first + std::to_string(i)).c_str(), &light.color[0])) lights_updated = true;
      if(ImGui::InputFloat(("strength: %f ##" + lights.first + std::to_string(i)).c_str(), &light.strength)) lights_updated = true;

      std::string delete_light_label = "Delete ##(" + std::to_string(i) + ")";
      if(ImGui::Button(delete_light_label.c_str())) {
        lights.second.erase(i);
        Logger::get().info("UI", "Deleted Light from {" + lights.first + "} at {" + std::to_string(i) + "}");
        lights_updated = true;
      };

      ImGui::Separator();
    };

    std::string add_light_label = "Add Light ##(" + std::to_string(lights.second.size()) + ")";
    if(ImGui::Button(add_light_label.c_str())) {
      lights.second.emplace_back(UW::Light({0, 0, 0}, {1, 1, 1}, 1));
      Logger::get().info("UI", "Added Light to {" + lights.first + "} at {" + std::to_string(lights.second.size()) + "}");
      lights_updated = true;
    };
  };

  if(lights_updated){
    DataSerializer::get().saveAllLights(Resources::get().lights);
    Logger::get().info("UI", "Lights saved");
  };
};



std::function<void(CW::Renderer::iRenderer *window)> UW::UI_Lights::ui(){
  return [this](CW::Renderer::iRenderer *window){
    guiLights();
  };
};



#endif
