// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "UI.h"
#ifndef PRODUCTION



Engine::Editor::UI::UI(CW::Renderer::Renderer &window, float &fps, Engine::Core::Scene& scene, CW::Renderer::Framebuffer& viewport_fbo)
  :window(window), gui(&window), scene(scene),
  info_ui(gui, fps, scene),
  log_ui(gui),
  materials_ui(gui),
  objects_ui(gui, window, scene),
  lights_ui(gui),
  shader_ui(gui),
  asset_loader_ui(gui, scene),
  scripts_ui(gui),
  viewport_ui(gui, viewport_fbo){
  Engine::Utils::Logger::get().info("UI", "Initializing UI");
  
  gui.setWorkspace(appWorkspace());
};



Engine::Editor::UI::~UI(){
  onDestroy();
};



void Engine::Editor::UI::onLoad(){
  Engine::Utils::Logger::get().info("UI", "Loading UI");

  uiLoad();
  window.setSize(guiSettings.window_width, Engine::Editor::guiSettings.window_height);
  Engine::Utils::Logger::get().info("UI", "Window Size Setted { "+ std::to_string(guiSettings.window_width) + " x " + std::to_string(guiSettings.window_height) +" }");
};



void Engine::Editor::UI::render(){
  gui.render();
};



void Engine::Editor::UI::onDestroy() {
  Engine::Utils::Logger::get().info("UI", "Destroying UI");
  scripts_ui.saveScriptEditors();
  shader_ui.saveShaderEditors();
};



// ========================= //
// ========== GUI ========== //
// ========================= //
void Engine::Editor::UI::uiLoad(){
  configControl();
  static std::string path_to_ini = Engine::Config::TEMP_BIN_FOLDER + ImGui::GetIO().IniFilename;
  
  if(!std::filesystem::exists(Engine::Config::TEMP_BIN_FOLDER)) std::filesystem::create_directories(Engine::Config::TEMP_BIN_FOLDER);

  ImGui::GetIO().IniFilename = path_to_ini.c_str();
  ImGui::LoadIniSettingsFromDisk(ImGui::GetIO().IniFilename);

  Engine::Utils::Logger::get().info("UI", "Loading UI Data from disck");

  Engine::Core::Resources::get().simulation_mode = Engine::Editor::guiSettings.simulation_mode;

  shader_ui.loadShaderEditors();
  scripts_ui.loadScriptEditors();

  uiControl();
};


void Engine::Editor::UI::configControl(){
  ImGuiSettingsHandler handler;
  handler.TypeName = "GuiSettings";
  handler.TypeHash = ImHashStr("GuiSettings");

  handler.ReadOpenFn = [](ImGuiContext*, ImGuiSettingsHandler*, const char*){
    return (void*)&Engine::Editor::guiSettings;
  };

  handler.ReadLineFn = [](ImGuiContext*, ImGuiSettingsHandler*, void* entry, const char* line){
    Engine::Editor::GuiSettings* s = (Engine::Editor::GuiSettings*)entry;

    int value;
    if (sscanf(line, "InfoWindowOn=%d", &value) == 1) s->infoWindowOn = value;
    if (sscanf(line, "LogWindowOn=%d", &value) == 1) s->logWindowOn = value;
    if (sscanf(line, "viewportWindowOn=%d", &value) == 1) s->viewportWindowOn = value;
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
    out_buf->appendf("InfoWindowOn=%d\n", Engine::Editor::guiSettings.infoWindowOn);
    out_buf->appendf("LogWindowOn=%d\n", Engine::Editor::guiSettings.logWindowOn);
    out_buf->appendf("viewportWindowOn=%d\n", Engine::Editor::guiSettings.viewportWindowOn);
    out_buf->appendf("MaterialExplorerOn=%d\n", Engine::Editor::guiSettings.materialExplorerOn);
    out_buf->appendf("LightsExplorerOn=%d\n", Engine::Editor::guiSettings.lightsExplorerOn);
    out_buf->appendf("MaterialEditorOn=%d\n", Engine::Editor::guiSettings.materialEditorOn);
    out_buf->appendf("ShaderExplorerWindowOn=%d\n", Engine::Editor::guiSettings.shaderExplorerWindowOn);
    out_buf->appendf("ScriptsExplorerWindowOn=%d\n", Engine::Editor::guiSettings.scriptsExplorerWindowOn);
    out_buf->appendf("ShaderEditorWindowOn=%d\n", Engine::Editor::guiSettings.shaderEditorWindowOn);
    out_buf->appendf("ScriptEditorWindowOn=%d\n", Engine::Editor::guiSettings.scriptEditorWindowOn);
    out_buf->appendf("ObjectExplorerWindowOn=%d\n", Engine::Editor::guiSettings.objectExplorerWindowOn);
    out_buf->appendf("ObjectEditorWindowOn=%d\n", Engine::Editor::guiSettings.objectEditorWindowOn);
    out_buf->appendf("Object_ID=%d\n", Engine::Editor::guiSettings.object_id);
    out_buf->appendf("Mesh_Mode_On=%d\n", Engine::Editor::guiSettings.mesh_mode_on);
    out_buf->appendf("Window_Width=%d\n", Engine::Editor::guiSettings.window_width);
    out_buf->appendf("Window_Height=%d\n", Engine::Editor::guiSettings.window_height);
    out_buf->appendf("Material_ID=%s\n", Engine::Editor::guiSettings.material_name.c_str());
    out_buf->appendf("Simulation_Mode=%d\n", Engine::Editor::guiSettings.simulation_mode);
    
    out_buf->appendf("ShaderEditorCount=%zu\n", Engine::Editor::guiSettings.shader_editors_reg.size());

    for (size_t i = 0; i < Engine::Editor::guiSettings.shader_editors_reg.size(); ++i){
      out_buf->appendf(
        "ShaderEditor=%s,%u\n",
        Engine::Editor::guiSettings.shader_editors_reg[i].first.c_str(),
        Engine::Editor::guiSettings.shader_editors_reg[i].second
      );
    };

    out_buf->appendf("ScriptEditorCount=%zu\n", Engine::Editor::guiSettings.scripts_editors_reg.size());

    for (size_t i = 0; i < Engine::Editor::guiSettings.scripts_editors_reg.size(); ++i){
      out_buf->appendf(
        "ScriptEditor=%s\n",
        Engine::Editor::guiSettings.scripts_editors_reg[i].c_str()
      );
    };

    out_buf->append("\n");
  };

  ImGui::GetCurrentContext()->SettingsHandlers.push_back(handler);
};



void Engine::Editor::UI::uiControl(){
  info_ui.uiControl();
  log_ui.uiControl();
  materials_ui.uiControl();
  objects_ui.uiControl();
  lights_ui.uiControl();
  shader_ui.uiControl();
  asset_loader_ui.uiControl();
  scripts_ui.uiControl();
  viewport_ui.uiControl();
};



void Engine::Editor::UI::menuBarGui(){
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("Window")) {
      if(ImGui::MenuItem("Info")){
        Engine::Editor::guiSettings.infoWindowOn = !Engine::Editor::guiSettings.infoWindowOn;
        uiControl();
      };
      if(ImGui::MenuItem("Logs")){
        Engine::Editor::guiSettings.logWindowOn = !Engine::Editor::guiSettings.logWindowOn;
        uiControl();
      };
      if(ImGui::MenuItem("Material Explorer")){
        Engine::Editor::guiSettings.materialExplorerOn = !Engine::Editor::guiSettings.materialExplorerOn;
        uiControl();
      };
      if(ImGui::MenuItem("Material Editor")){
        Engine::Editor::guiSettings.materialEditorOn = !Engine::Editor::guiSettings.materialEditorOn;
        uiControl();
      };
      if(ImGui::MenuItem("Lights Explorer")){
        Engine::Editor::guiSettings.lightsExplorerOn = !Engine::Editor::guiSettings.lightsExplorerOn;
        uiControl();
      };
      if(ImGui::MenuItem("Shader Explorer")){
        Engine::Editor::guiSettings.shaderExplorerWindowOn = !Engine::Editor::guiSettings.shaderExplorerWindowOn;
        uiControl();
      };
      if(ImGui::MenuItem("Script Explorer")){
        Engine::Editor::guiSettings.scriptsExplorerWindowOn = !Engine::Editor::guiSettings.scriptsExplorerWindowOn;
        uiControl();
      };
      if(ImGui::MenuItem("Object Explorer")){
        Engine::Editor::guiSettings.objectExplorerWindowOn = !Engine::Editor::guiSettings.objectExplorerWindowOn;
        uiControl();
      };
      if(ImGui::MenuItem("Object Editor")){
        Engine::Editor::guiSettings.objectEditorWindowOn = !Engine::Editor::guiSettings.objectEditorWindowOn;
        uiControl();
      };
      if(ImGui::MenuItem("Viewport")){
        Engine::Editor::guiSettings.viewportWindowOn = !Engine::Editor::guiSettings.viewportWindowOn;
        uiControl();
      };
      ImGui::EndMenu();
    };

    if(ImGui::BeginMenu("Assets")){
      if(ImGui::MenuItem("Asset Loader")){
        Engine::Editor::guiSettings.assetLoaderWindowOn = !Engine::Editor::guiSettings.assetLoaderWindowOn;
        uiControl();
      };
      ImGui::EndMenu();
    };

    if(ImGui::BeginMenu("Properties")){
      char title_buffer[Engine::Config::OBJECT_NAME_BUFFER_SIZE] = {};
      memcpy(title_buffer, Engine::ScriptShared::GlobResource::get().WINDOW_TITLE.data(), Engine::ScriptShared::GlobResource::get().WINDOW_TITLE.size());
      if(ImGui::InputText("Window Title", title_buffer, Engine::Config::OBJECT_NAME_BUFFER_SIZE)){
        Engine::ScriptShared::GlobResource::get().WINDOW_TITLE = std::string(title_buffer);
      };
      
      bool vsync_on = Engine::ScriptShared::GlobResource::get().VSYNC;
      if(ImGui::Checkbox("Vsync", &vsync_on)) Engine::ScriptShared::GlobResource::get().VSYNC = vsync_on;

      float fixed_hz = Engine::ScriptShared::GlobResource::get().FIXED_HZ;
      if(ImGui::InputFloat("Fixed_HZ", &fixed_hz)){
        Engine::ScriptShared::GlobResource::get().FIXED_HZ = fixed_hz;
      };

      ImGui::EndMenu();
    };
    
    bool new_simulation_mode = Engine::Core::Resources::get().simulation_mode;
    if(ImGui::Checkbox("Simulation", &new_simulation_mode)){
      Engine::Core::Resources::get().simulation_mode = new_simulation_mode;
      Engine::Editor::guiSettings.simulation_mode = new_simulation_mode;
    };

    if(ImGui::Button("Build")) buildProject();
    if(ImGui::Button("Run")) runProject();

    ImGui::EndMenuBar();
  };
};



void Engine::Editor::UI::buildProject(){
  #ifndef PRODUCTION
  Engine::Utils::Logger::get().info("UI", "Building ...");
  
  std::filesystem::path dest_folder = Engine::Config::TEMP_BIN_FOLDER + Engine::Config::COMPILATION_FOLDER;

  if(!std::filesystem::exists(Engine::Config::TEMP_BIN_FOLDER)) std::filesystem::create_directories(Engine::Config::TEMP_BIN_FOLDER);

  std::string build_dir = (dest_folder / "build-prod").string();
  
  // clean stage
  if(std::filesystem::exists(dest_folder)){
    std::filesystem::remove_all(dest_folder / Engine::Config::GAME_DATA_FOLDER);
    std::filesystem::remove_all(dest_folder / Engine::Config::SCRIPTS_SRC_FOLDER);
  }
  else{
    // engine unzip
    try {
      std::filesystem::path system_zip_path = std::filesystem::path(ENGINE_DATA_ZIP);

      if (!std::filesystem::exists(system_zip_path)) {
        Engine::Utils::Logger::get().erro("UI", "System ZIP file not found at: " + system_zip_path.string());
        return;
      };

      const char* unzip_args[] = {
        "unzip",
        "-o",
        "-q",
        system_zip_path.c_str(),
        "-d",
        dest_folder.c_str(),
        nullptr
      };

      pid_t unzip_pid = fork();
      if (unzip_pid == 0) {
        execvp("unzip", const_cast<char* const*>(unzip_args));
        Engine::Utils::Logger::get().erro("UI", "Failed to exec unzip for extraction");
        _exit(-1);
      } else if (unzip_pid > 0) {
        int status = 0;
        waitpid(unzip_pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
          Engine::Utils::Logger::get().erro("UI", "Failed to extract engine_data.zip!");
          return;
        };
      };
      
      Engine::Utils::Logger::get().info("UI", "Engine data successfully extracted from ZIP.");
    } catch (const std::exception& e) {
      Engine::Utils::Logger::get().erro("UI", std::string("CMRC Zip extraction failed: ") + e.what());
      return;
    };
  };

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
  std::string generator = GENERATOR;
  const char* config[] = {
    "cmake",
    "-S",
    dest_folder.c_str(), 
    "-B",
    build_dir.c_str(),
    "-G", 
    generator.c_str(),
    "-DPRODUCTION=ON",
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

  std::filesystem::copy(dest_folder / "build-prod" / "Engine" / "App" / "App", "App", std::filesystem::copy_options::overwrite_existing);
  
  
  Engine::Utils::Logger::get().info("UI", "Project Builded");
#else
  Engine::Utils::Logger::get().info("UI", "In PRODUCTION mode: cmrc extraction skipped.");
#endif
};



void Engine::Editor::UI::runProject(){
  Engine::DataSerializer::get().saveAll();

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



std::function<void(std::function<void()> render_windows)> Engine::Editor::UI::appWorkspace() {
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
