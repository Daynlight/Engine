// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "UI.h"
#ifndef PRODUCTION

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(EngineData);



UW::UI::UI(CW::Renderer::Renderer &window, float &fps, UW::Scene& scene)
  :window(window), gui(&window), scene(scene),
  info_ui(gui, fps, scene),
  log_ui(gui),
  materials_ui(gui),
  objects_ui(gui, window, scene),
  lights_ui(gui),
  shader_ui(gui),
  asset_loader_ui(gui, scene),
  scripts_ui(gui){
  Engine::Utils::Logger::get().info("UI", "Initializing UI");
  
  gui.setWorkspace(appWorkspace());
};



UW::UI::~UI(){
  onDestroy();
};



void UW::UI::onLoad(){
  Engine::Utils::Logger::get().info("UI", "Loading UI");

  uiLoad();
  window.setSize(guiSettings.window_width, guiSettings.window_height);
  Engine::Utils::Logger::get().info("UI", "Window Size Setted { "+ std::to_string(guiSettings.window_width) + " x " + std::to_string(guiSettings.window_height) +" }");
};



void UW::UI::render(){
  gui.render();
};



void UW::UI::onDestroy() {
  Engine::Utils::Logger::get().info("UI", "Destroying UI");
  scripts_ui.saveScriptEditors();
  shader_ui.saveShaderEditors();
};



// ========================= //
// ========== GUI ========== //
// ========================= //
void UW::UI::uiLoad(){
  configControl();
  ImGui::LoadIniSettingsFromDisk(ImGui::GetIO().IniFilename);
  Engine::Utils::Logger::get().info("UI", "Loading UI Data from disck");

  Resources::get().simulation_mode = guiSettings.simulation_mode;

  shader_ui.loadShaderEditors();
  scripts_ui.loadScriptEditors();

  uiControl();
};


void UW::UI::configControl(){
  ImGuiSettingsHandler handler;
  handler.TypeName = "GuiSettings";
  handler.TypeHash = ImHashStr("GuiSettings");

  handler.ReadOpenFn = [](ImGuiContext*, ImGuiSettingsHandler*, const char*){
    return (void*)&UW::guiSettings;
  };

  handler.ReadLineFn = [](ImGuiContext*, ImGuiSettingsHandler*, void* entry, const char* line){
    GuiSettings* s = (GuiSettings*)entry;

    int value;
    if (sscanf(line, "InfoWindowOn=%d", &value) == 1) s->infoWindowOn = value;
    if (sscanf(line, "LogWindowOn=%d", &value) == 1) s->logWindowOn = value;
    if (sscanf(line, "MaterialExplorerOn=%d", &value) == 1) s->materialExplorerOn = value;
    if (sscanf(line, "LightsExplorerOn=%d", &value) == 1) s->lightsExplorerOn = value;
    if (sscanf(line, "MaterialEditorOn=%d", &value) == 1) s->materialEditorOn = value;
    if (sscanf(line, "ShaderExplorerWindowOn=%d", &value) == 1) s->shaderExplorerWindowOn = value;
    if (sscanf(line, "ScriptsExplorerWindowOn=%d", &value) == 1) s->scriptsExplorerWindowOn= value;
    if (sscanf(line, "ShaderEditorWindowOn=%d", &value) == 1) s->shaderEditorWindowOn = value;
    if (sscanf(line, "ScriptEditorWindowOn=%d", &value) == 1) s->scriptEditorWindowOn = value;
    if (sscanf(line, "ObjectExplorerWindowOn=%d", &value) == 1) s->objectExplorerWindowOn = value;
    if (sscanf(line, "ObjectEditorWindowOn=%d", &value) == 1) s->objectEditorWindowOn = value;
    if (sscanf(line, "Object_ID=%d", &value) == 1) s->object_id = value;
    if (sscanf(line, "Mesh_Mode_On=%d", &value) == 1) s->mesh_mode_on = value;
    if (sscanf(line, "Window_Width=%d", &value) == 1) s->window_width = value;
    if (sscanf(line, "Window_Height=%d", &value) == 1) s->window_height = value;
    if (sscanf(line, "Simulation_Mode=%d", &value) == 1) s->simulation_mode = value;
    
    char value_str[256];
    if (sscanf(line, "Material_ID=%255s", &value_str) == 1) s->material_name = std::string(value_str);
    
    char name[256];
    unsigned int type;

    if (sscanf(line, "ShaderEditor=%255[^,],%u", name, &type) == 2){
      s->shader_editors_reg.emplace_back(name, type);
    };

    if (sscanf(line, "ScriptEditor=%255[^,]", name) == 1){
      s->scripts_editors_reg.emplace_back(name);
    };
  };

  handler.WriteAllFn = [](ImGuiContext*, ImGuiSettingsHandler* handler, ImGuiTextBuffer* out_buf){
    out_buf->appendf("[%s][Main]\n", handler->TypeName);
    out_buf->appendf("InfoWindowOn=%d\n", guiSettings.infoWindowOn);
    out_buf->appendf("LogWindowOn=%d\n", guiSettings.logWindowOn);
    out_buf->appendf("MaterialExplorerOn=%d\n", guiSettings.materialExplorerOn);
    out_buf->appendf("LightsExplorerOn=%d\n", guiSettings.lightsExplorerOn);
    out_buf->appendf("MaterialEditorOn=%d\n", guiSettings.materialEditorOn);
    out_buf->appendf("ShaderExplorerWindowOn=%d\n", guiSettings.shaderExplorerWindowOn);
    out_buf->appendf("ScriptsExplorerWindowOn=%d\n", guiSettings.scriptsExplorerWindowOn);
    out_buf->appendf("ShaderEditorWindowOn=%d\n", guiSettings.shaderEditorWindowOn);
    out_buf->appendf("ScriptEditorWindowOn=%d\n", guiSettings.scriptEditorWindowOn);
    out_buf->appendf("ObjectExplorerWindowOn=%d\n", guiSettings.objectExplorerWindowOn);
    out_buf->appendf("ObjectEditorWindowOn=%d\n", guiSettings.objectEditorWindowOn);
    out_buf->appendf("Object_ID=%d\n", guiSettings.object_id);
    out_buf->appendf("Mesh_Mode_On=%d\n", guiSettings.mesh_mode_on);
    out_buf->appendf("Window_Width=%d\n", guiSettings.window_width);
    out_buf->appendf("Window_Height=%d\n", guiSettings.window_height);
    out_buf->appendf("Material_ID=%s\n", guiSettings.material_name.c_str());
    out_buf->appendf("Simulation_Mode=%d\n", guiSettings.simulation_mode);
    
    out_buf->appendf("ShaderEditorCount=%zu\n", guiSettings.shader_editors_reg.size());

    for (size_t i = 0; i < guiSettings.shader_editors_reg.size(); ++i){
      out_buf->appendf(
        "ShaderEditor=%s,%u\n",
        guiSettings.shader_editors_reg[i].first.c_str(),
        guiSettings.shader_editors_reg[i].second
      );
    };

    out_buf->appendf("ScriptEditorCount=%zu\n", guiSettings.scripts_editors_reg.size());

    for (size_t i = 0; i < guiSettings.scripts_editors_reg.size(); ++i){
      out_buf->appendf(
        "ScriptEditor=%s\n",
        guiSettings.scripts_editors_reg[i].c_str()
      );
    };

    out_buf->append("\n");
  };

  ImGui::GetCurrentContext()->SettingsHandlers.push_back(handler);
};



void UW::UI::uiControl(){
  info_ui.uiControl();
  log_ui.uiControl();
  materials_ui.uiControl();
  objects_ui.uiControl();
  lights_ui.uiControl();
  shader_ui.uiControl();
  asset_loader_ui.uiControl();
  scripts_ui.uiControl();
};



void UW::UI::menuBarGui(){
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("Window")) {
      if(ImGui::MenuItem("Info")){
        guiSettings.infoWindowOn = !guiSettings.infoWindowOn;
        uiControl();
      };
      if(ImGui::MenuItem("Logs")){
        guiSettings.logWindowOn = !guiSettings.logWindowOn;
        uiControl();
      };
      if(ImGui::MenuItem("Material Explorer")){
        guiSettings.materialExplorerOn = !guiSettings.materialExplorerOn;
        uiControl();
      };
      if(ImGui::MenuItem("Material Editor")){
        guiSettings.materialEditorOn = !guiSettings.materialEditorOn;
        uiControl();
      };
      if(ImGui::MenuItem("Lights Explorer")){
        guiSettings.lightsExplorerOn = !guiSettings.lightsExplorerOn;
        uiControl();
      };
      if(ImGui::MenuItem("Shader Explorer")){
        guiSettings.shaderExplorerWindowOn = !guiSettings.shaderExplorerWindowOn;
        uiControl();
      };
      if(ImGui::MenuItem("Script Explorer")){
        guiSettings.scriptsExplorerWindowOn = !guiSettings.scriptsExplorerWindowOn;
        uiControl();
      };
      if(ImGui::MenuItem("Object Explorer")){
        guiSettings.objectExplorerWindowOn = !guiSettings.objectExplorerWindowOn;
        uiControl();
      };
      if(ImGui::MenuItem("Object Editor")){
        guiSettings.objectEditorWindowOn = !guiSettings.objectEditorWindowOn;
        uiControl();
      };
      ImGui::EndMenu();
    };

    if(ImGui::BeginMenu("Assets")){
      if(ImGui::MenuItem("Asset Loader")){
        guiSettings.assetLoaderWindowOn = !guiSettings.assetLoaderWindowOn;
        uiControl();
      };
      ImGui::EndMenu();
    };

    if(ImGui::BeginMenu("Properties")){
      char title_buffer[Engine::Config::OBJECT_NAME_BUFFER_SIZE] = {};
      memcpy(title_buffer, UW::GlobResource::get().WINDOW_TITLE.data(), UW::GlobResource::get().WINDOW_TITLE.size());
      if(ImGui::InputText("Window Title", title_buffer, Engine::Config::OBJECT_NAME_BUFFER_SIZE)){
        UW::GlobResource::get().WINDOW_TITLE = std::string(title_buffer);
      };
      
      bool vsync_on = UW::GlobResource::get().VSYNC;
      if(ImGui::Checkbox("Vsync", &vsync_on)) UW::GlobResource::get().VSYNC = vsync_on;

      float fixed_hz = UW::GlobResource::get().FIXED_HZ;
      if(ImGui::InputFloat("Fixed_HZ", &fixed_hz)){
        UW::GlobResource::get().FIXED_HZ = fixed_hz;
      };

      ImGui::EndMenu();
    };
    
    bool new_simulation_mode = Resources::get().simulation_mode;
    if(ImGui::Checkbox("Simulation", &new_simulation_mode)){
      Resources::get().simulation_mode = new_simulation_mode;
      guiSettings.simulation_mode = new_simulation_mode;
    };

    if(ImGui::Button("Build")) buildProject();
    if(ImGui::Button("Run")) runProject();

    ImGui::EndMenuBar();
  };
};



void UW::UI::buildProject(){
  #ifndef PRODUCTION
  Engine::Utils::Logger::get().info("UI", "Building ...");
  
  std::filesystem::path dest_folder = Engine::Config::COMPILATION_FOLDER;
  std::string build_dir = (dest_folder / "build").string();
  
  // clean stage
  if(std::filesystem::exists(dest_folder)){
    std::filesystem::remove_all(dest_folder / Engine::Config::GAME_DATA_FOLDER);
    std::filesystem::remove_all(dest_folder / Engine::Config::SCRIPTS_SRC_FOLDER);
  };

  
  // copy stage
  auto efs = cmrc::EngineData::get_filesystem();

  auto extract_dir = [&](auto& self, const std::string& virtual_path, const std::filesystem::path& physical_path) -> void { 
    if (!std::filesystem::exists(physical_path)) std::filesystem::create_directories(physical_path);

    for (auto&& entry : efs.iterate_directory(virtual_path)) {
      std::string current_v_path = virtual_path.empty() ? entry.filename() : virtual_path + "/" + entry.filename();
      std::filesystem::path current_p_path = physical_path / entry.filename();
      if(std::filesystem::exists(current_p_path)) continue;

      if (entry.is_directory()) self(self, current_v_path, current_p_path);
      else if (entry.is_file()) {
        auto file = efs.open(current_v_path);
        bool should_write = true;

        if (std::filesystem::exists(current_p_path) && std::filesystem::file_size(current_p_path) == file.size()) should_write = false; 

        if (should_write) {
          std::ofstream out(current_p_path, std::ios::binary);
          if (out) out.write(file.begin(), file.size());
          else Engine::Utils::Logger::get().erro("UI", "Failed to write extracted file: " + current_p_path.string());
        };
      };
    };
  };

  extract_dir(extract_dir, "", dest_folder);
  Engine::Utils::Logger::get().info("UI", "Engine data successfully initialized from cmrc.");
  
  // copying game data
  std::filesystem::copy(
    Engine::Config::GAME_DATA_FOLDER,
    dest_folder / Engine::Config::GAME_DATA_FOLDER,
    std::filesystem::copy_options::recursive |
    std::filesystem::copy_options::overwrite_existing
  );

  std::filesystem::copy(
    Engine::Config::SCRIPTS_SRC_FOLDER,
    dest_folder / Engine::Config::SCRIPTS_SRC_FOLDER,
    std::filesystem::copy_options::recursive |
    std::filesystem::copy_options::overwrite_existing
  );

  // compilation stage
  const char* config[] = {
    "cmake",
    "-S",
    dest_folder.c_str(), 
    "-B",
    build_dir.c_str(),
    nullptr
  };

  const char* build[] = {
    "cmake",
    "--build", 
    build_dir.c_str(),
    "--target", 
    "App",
    nullptr
  };

  pid_t pid = fork();
  if(pid == 0){
    execvp("cmake", const_cast<char* const*>(config)); 
    Engine::Utils::Logger::get().erro("UI", "Failed to exec g++");
    exit(-1);
  }
  else if(pid > 0){
    int status = 0; 
    
    waitpid(pid, &status, 0);
    
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
      Engine::Utils::Logger::get().info("UI", "successful compilation");
      // return; 
    } 
    else { 
      Engine::Utils::Logger::get().erro("UI", "Compilation failed!");
      return; 
    };
  };

  pid_t pid1 = fork();
  if(pid1 == 0){
    execvp("cmake", const_cast<char* const*>(build)); 
    Engine::Utils::Logger::get().erro("UI", "Failed to exec g++");
    exit(-1);
  }
  else if(pid1 > 0){
    int status = 0; 
    
    waitpid(pid1, &status, 0);
    
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
      Engine::Utils::Logger::get().info("UI", "successful compilation");
      // return; 
    } 
    else { 
      Engine::Utils::Logger::get().erro("UI", "Compilation failed!");
      return; 
    };
  };

  std::filesystem::copy(dest_folder / "build" / "Engine" / "App" / "App", "App", std::filesystem::copy_options::overwrite_existing);
  
  
  Engine::Utils::Logger::get().info("UI", "Project Builded");
#else
  Engine::Utils::Logger::get().info("UI", "In PRODUCTION mode: cmrc extraction skipped.");
#endif
};



void UW::UI::runProject(){
  UW::DataSerializer::get().saveAll();

  buildProject();

  const char* run[] = { 
    "./App",
    nullptr
  };
  
  pid_t pid1 = fork();
  if(pid1 == 0){
    execvp("./App", const_cast<char* const*>(run)); 
    Engine::Utils::Logger::get().erro("UI", "Failed to exec g++");
    exit(-1);
  }
  else if(pid1 > 0){
    int status = 0; 
    
    waitpid(pid1, &status, 0);
    
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
      Engine::Utils::Logger::get().info("UI", "successful compilation");
      // return; 
    } 
    else { 
      Engine::Utils::Logger::get().erro("UI", "Compilation failed!");
      return; 
    };
  };
};



std::function<void(std::function<void()> render_windows)> UW::UI::appWorkspace() {
  return [this](std::function<void()> render_windows){
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | 
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
                        
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); 
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    
    ImGui::Begin("Window DockSpace", nullptr, window_flags);
    
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();

    menuBarGui();

    ImGuiID docspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(docspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    render_windows();

    ImGui::End();
  };
};

#endif
