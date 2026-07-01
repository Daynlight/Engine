// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "ScriptController.h"



#ifndef PRODUCTION
#include <cmrc/cmrc.hpp>
CMRC_DECLARE(ScriptShared);
#endif



void UW::GameObjectScriptRecord::initSharedFolder() {
#ifndef PRODUCTION
  auto efs = cmrc::ScriptShared::get_filesystem();
  std::filesystem::path dest_folder = UW::Config::SCRIPTS_SRC_FOLDER;

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
          else UW::Logger::get().erro("Script Controller", "Failed to write extracted file: " + current_p_path.string());
        };
      };
    };
  };

  extract_dir(extract_dir, "", dest_folder);
  UW::Logger::get().info("Script Controller", "Shared folder successfully initialized from cmrc.");
#else
  UW::Logger::get().info("Script Controller", "In PRODUCTION mode: cmrc extraction skipped.");
#endif
};



UW::GameObjectScriptRecord::GameObjectScriptRecord(const std::string &path)
  : path(path), 
    cpp_file(UW::Config::SCRIPTS_SRC_FOLDER + path + ".cpp"),
#if defined(_WIN32) || defined(_WIN64)
    so_file(UW::Config::SCRIPTS_DLL_FOLDER + path + ".dll")
#else
    so_file(UW::Config::SCRIPTS_DLL_FOLDER + path + ".so")
#endif 
{
  UW::Logger::get().info("Script Controller", "Script Initialized");
  initSharedFolder();
};



UW::GameObjectScriptRecord::~GameObjectScriptRecord(){
  UW::Logger::get().info("Script Controller", "Script Destroying");
  removeModule();
  UW::Logger::get().info("Script Controller", "Script Destroyed");
};



UW::GameObjectScriptRecord::GameObjectScriptRecord(const GameObjectScriptRecord& other)
  : path(other.path), cpp_file(other.cpp_file), so_file(other.so_file), log_observe_lock(other.log_observe_lock) {
  script_handler = nullptr;
  script = nullptr;
  script_on = other.script_on;
};



UW::GameObjectScriptRecord& UW::GameObjectScriptRecord::operator=(const GameObjectScriptRecord& other) {
  if (this != &other) {
    removeModule();
    path = other.path;
    cpp_file = other.cpp_file;
    so_file = other.so_file;
    log_observe_lock = other.log_observe_lock;
    script_handler = nullptr;
    script = nullptr;
    script_on = other.script_on;
  };
  return *this;
};



UW::GameObjectScriptRecord::GameObjectScriptRecord(GameObjectScriptRecord&& other) noexcept
  : path(std::move(other.path)), cpp_file(std::move(other.cpp_file)),
    so_file(std::move(other.so_file)), log_observe_lock(other.log_observe_lock),
    script_handler(other.script_handler), script(other.script) {
  other.script_handler = nullptr;
  other.script = nullptr;
  script_on = other.script_on;
};



UW::GameObjectScriptRecord& UW::GameObjectScriptRecord::operator=(GameObjectScriptRecord&& other) noexcept {
  if (this != &other) {
    removeModule();
    path = std::move(other.path);
    cpp_file = std::move(other.cpp_file);
    so_file = std::move(other.so_file);
    log_observe_lock = other.log_observe_lock;
    script_handler = other.script_handler;
    script = other.script;

    other.script_handler = nullptr;
    other.script = nullptr;
    script_on = other.script_on;
  };
  return *this;
};



void UW::GameObjectScriptRecord::syncPointer(GameObjectData* data) {
  if (script) script->game_object_data = data;
};



void UW::GameObjectScriptRecord::observe(GameObjectData *data){
#ifndef PRODUCTION
  if(compiling) {
    checkLastWrite();
    updateScript(data);
    return;
  };

  if(checkLastWrite()) updateScript(data);
#else
  if(!module_initialized) 
    if(!loadModule()) 
      onLoad(data);
#endif
};



void UW::GameObjectScriptRecord::onLoad(GameObjectData* data) {
  if(script){
    script->game_object_data = data;
    script->glob_res = &UW::GlobResource::get();
    script->logger = static_cast<ILogger*>(&UW::Logger::get());
    script->object_manager = static_cast<IObjectManager*>(&UW::ObjectManager::get());

#ifndef PRODUCTION
#ifdef SANDBOX_SCRIPTS
#if !defined(_WIN32) && !defined(_WIN64)
    pid_t pid = fork();
    if(pid == 0){
      script->OnLoad();

      _exit(0);
    }
    else if(pid > 0){
      int status = 0; 
      waitpid(pid, &status, 0);

      if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) { 
        UW::Logger::get().erro("Script Controller", std::to_string(status) + " - Init failed!");
        return; 
      };
    };
#endif
#endif
#endif

    script->OnLoad();
  };
};



void UW::GameObjectScriptRecord::onUpdate(float delta_time) {
  if(!script_on) return;

  if(script){

#ifndef PRODUCTION
#ifdef SANDBOX_SCRIPTS
#if !defined(_WIN32) && !defined(_WIN64)
    pid_t pid = fork();
    if(pid == 0){
      script->OnUpdate(delta_time);

      _exit(0);
    }
    else if(pid > 0){
      int status = 0; 
      waitpid(pid, &status, 0);

      if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) { 
        UW::Logger::get().erro("Script Controller", std::to_string(status) + " - Update failed!");
        return; 
      };
    };
#endif
#endif
#endif

    script->OnUpdate(delta_time);
  };
};



void UW::GameObjectScriptRecord::onFixedUpdate(float fixed_delta_time) {
  if(!script_on) return;

  if(script){

#ifndef PRODUCTION
#ifdef SANDBOX_SCRIPTS
#if !defined(_WIN32) && !defined(_WIN64)
    pid_t pid = fork();
    if(pid == 0){
      script->OnFixedUpdate(fixed_delta_time);

      _exit(0);
    }
    else if(pid > 0){
      int status = 0; 
      waitpid(pid, &status, 0);

      if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) { 
        UW::Logger::get().erro("Script Controller", std::to_string(status) + " - FixedUpdate failed!");
        return; 
      };
    };
#endif
#endif
#endif

    script->OnFixedUpdate(fixed_delta_time);
  };
};



void UW::GameObjectScriptRecord::onRender() {
  if(!script_on) return;

  if(script){

#ifndef PRODUCTION
#ifdef SANDBOX_SCRIPTS
#if !defined(_WIN32) && !defined(_WIN64)
    pid_t pid = fork();
    if(pid == 0){
      script->OnRender();

      _exit(0);
    }
    else if(pid > 0){
      int status = 0; 
      waitpid(pid, &status, 0);

      if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) { 
        UW::Logger::get().erro("Script Controller", std::to_string(status) + " - Render failed!");
        return; 
      };
    };
#endif
#endif
#endif

    script->OnRender();
  };
};



void UW::GameObjectScriptRecord::onDestroy() {
  if(script){

#ifndef PRODUCTION
#ifdef SANDBOX_SCRIPTS
#if !defined(_WIN32) && !defined(_WIN64)
    pid_t pid = fork();
    if(pid == 0){
      script->OnDestroy();

      _exit(0);
    }
    else if(pid > 0){
      int status = 0; 
      waitpid(pid, &status, 0);

      if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) { 
        UW::Logger::get().erro("Script Controller", std::to_string(status) + " - Destroy failed!");
        return; 
      };
    };
#endif
#endif
#endif

    script->OnDestroy();
  };
};



std::string UW::GameObjectScriptRecord::getPath() const {
  return path;
};



int UW::GameObjectScriptRecord::loadModule() {
  UW::Logger::get().info("Script Controller", "Module loading");

  removeModule();

#ifdef PRODUCTION
  script = UW::ScriptRegistry::get().createScript(path);
  
  if (!script) {
    UW::Logger::get().erro("Script Controller", "Script not found in registry: " + path);
    return -1;
  };
  
  module_initialized = 1;
  UW::Logger::get().info("Script Controller", "Module Loaded statically");
  return 0;

#else

  bool file_exist = std::filesystem::exists(so_file);
  if(!file_exist) return -1;

#if defined(_WIN32) || defined(_WIN64)
  script_handler = LoadLibraryA(so_file.c_str());
  if (!script_handler) {
    Logger::get().erro("Script Controller", "Failed to load DLL - Error Code: " + std::to_string(GetLastError()));
    return -1;
  }

  typedef GameObjectScriptInterface* (*GetScriptFunc)();
  GetScriptFunc getScript = (GetScriptFunc)GetProcAddress((HMODULE)script_handler, "GetScript");
  
  if (!getScript) {
    Logger::get().erro("Script Controller", "Cannot load symbol 'GetScript' - Error Code: " + std::to_string(GetLastError()));
    removeModule();
    return -1;
  }
#else
  script_handler = dlopen((so_file).c_str(), RTLD_NOW);

  if (!script_handler) {
    Logger::get().erro("Script Controller", "Failed to load script - " + std::string(dlerror()));
    return -1;
  };

  dlerror();

  typedef GameObjectScriptInterface* (*GetScriptFunc)();
  GetScriptFunc getScript = (GetScriptFunc)dlsym(script_handler, "GetScript");
  const char* dlsym_error = dlerror();
  
  if (dlsym_error || !getScript) {
    Logger::get().erro("Script Controller", "Cannot load symbol 'GetScript' - " + std::string(dlsym_error));
    removeModule();
    return -1;
  };
#endif

  script = getScript();

  if(!script){
    removeModule();
    UW::Logger::get().erro("Script Controller", "Script load failed");
    return -1;
  };

#endif

  Logger::get().info("Script Controller", "Module Loaded");
  return 0;
};



void UW::GameObjectScriptRecord::removeModule(){
  UW::Logger::get().info("Script Controller", "Module destroying");

#ifdef PRODUCTION
  if (script) {
    onDestroy();
    delete script;
    script = nullptr;
  };

  module_initialized = 0;

#else

  if (!script_handler) {
    script = nullptr;
    UW::Logger::get().info("Script Controller", "Module doesn't exists");
    return;
  };


  if(script){
    onDestroy();

#if defined(_WIN32) || defined(_WIN64)
    using DeleteScriptFunc = void (*)(GameObjectScriptInterface*);
    DeleteScriptFunc deleteScript = (DeleteScriptFunc)GetProcAddress((HMODULE)script_handler, "DeleteScript");

    if (!deleteScript) {
      Logger::get().warn("Script Controller", "DeleteScript not found or invalid");
      script = nullptr;
    } else {
      deleteScript(script);
      script = nullptr;
    }
#else
    dlerror();
    using DeleteScriptFunc = void (*)(GameObjectScriptInterface*);
    DeleteScriptFunc deleteScript = (DeleteScriptFunc)dlsym(script_handler, "DeleteScript");
    const char* dlsym_error = dlerror();

    if (dlsym_error || !deleteScript) {
    Logger::get().warn("Script Controller", "DeleteScript not found or invalid - " + std::string(dlsym_error ? dlsym_error : "null"));
      script = nullptr;
    }
    else{
      deleteScript(script);
      script = nullptr;
    };
#endif
  };

  
#if defined(_WIN32) || defined(_WIN64)
  if (script_handler) FreeLibrary((HMODULE)script_handler);
#else
  if(script_handler) dlclose(script_handler);
#endif
  script_handler = nullptr;
  script = nullptr;

#endif

  UW::Logger::get().info("Script Controller", "Module destroyed");
};



bool UW::GameObjectScriptRecord::checkLastWrite(){
#ifndef PRODUCTION
  bool file_exist = std::filesystem::exists(cpp_file);

  bool changed = 0;

  if(log_observe_lock && !file_exist){
    UW::Logger::get().erro("Script Controller", "No file named: " + cpp_file);
    log_observe_lock = 0;
  };
  
  if(!file_exist) return 0;
  
  log_observe_lock = 1;
  std::filesystem::file_time_type currentWriteTime{};

  try{
    currentWriteTime = std::filesystem::last_write_time(cpp_file);
  } catch(const std::filesystem::filesystem_error& e){
    UW::Logger::get().erro("Script Controller", "Filesystem error - " + std::string(e.what()));
    return false;
  };

  if(currentWriteTime != lastWriteTime){
    changed = 1;
    lastWriteTime = currentWriteTime;
    Logger::get().info("Script Controller", "Script changed lastWriteTime != currentWriteTime");
  };

  return changed;

#endif

  return 0;
};



void UW::GameObjectScriptRecord::updateScript(GameObjectData* data) {
#ifndef PRODUCTION
  UW::Logger::get().info("Script Controller", "Script is Updating...");

  removeModule();

  int compile_state = compile();
  if(compile_state == 0) {
    if(!loadModule()) 
      onLoad(data);
    else{
      UW::Logger::get().info("Script Controller", "loadModule failed");
      return;
    };

    compiling = 0;
  }
  else if(compile_state == 1 || compile_state == 2){
    compiling = 1;
  }
  else{
    UW::Logger::get().info("Script Controller", "compilation failed");
    return;
  };

  UW::Logger::get().info("Script Controller", "Script Updated");
#endif
};



int UW::GameObjectScriptRecord::compile() {
#ifndef PRODUCTION
  auto& resources = Resources::get();

  {
    std::lock_guard<std::mutex> lock(resources.compiler_mutex);
    for (const auto& finished_path : resources.completed_compilation_paths) {
      resources.script_active_compilers.erase(finished_path);
    };
    resources.completed_compilation_paths.clear();
  };

  auto it_write = resources.scripts_last_time_write.find(path);
  auto it_thread = resources.script_active_compilers.find(path);

  bool is_up_to_date = (it_write != resources.scripts_last_time_write.end() && lastWriteTime == it_write->second);
  bool is_compiling  = (it_thread != resources.script_active_compilers.end());

  if (is_up_to_date) {
    if (is_compiling) {
      Logger::get().info("Script Controller", "Compilation Exist Check: Still working in background...");
      return 1;
    } else {
      Logger::get().info("Script Controller", "Compilation Exist Check: Up to date, skipping.");
      return 0;
    };
  } 
  else {
    if (is_compiling) {
      Logger::get().info("Script Controller", "Compilation Reload: New changes saved while compiling! Waiting for current pass...");
      return 1;
    } 
    else {
      Logger::get().info("Script Controller", "Compilation Start: Kicking off new compilation thread.");

      resources.script_active_compilers[path] = std::jthread([this]() {

        int status = compile_thread();

        auto& res = Resources::get();
        if (status == 0) {
          std::lock_guard<std::mutex> lock(res.compiler_mutex);
          res.scripts_last_time_write[this->path] = this->lastWriteTime;
        };

        res.mark_as_completed(this->path);
      });

      return 2;
    };
  };
#endif
  return 0;
};



int UW::GameObjectScriptRecord::compile_thread(){
  std::filesystem::path p(so_file);
  std::filesystem::path dir = p.parent_path();

  if (!dir.empty() && !std::filesystem::exists(dir)) std::filesystem::create_directories(dir);

  std::filesystem::path compiler(COMPILER_PATH);
  std::filesystem::path so(so_file);
  std::filesystem::path cpp(cpp_file);

#if defined(_WIN32) || defined(_WIN64)
  std::filesystem::path temp_so = so.string() + ".tmp";

  STARTUPINFOA si = { sizeof(si) };
  PROCESS_INFORMATION pi = { 0 };

  std::string cmd = compiler.string() + " -shared -o \"" + temp_so.string() + "\" \"" + cpp.string() + "\"";
  Logger::get().warn("Script Controller", "Compile command: " + cmd);

  UW::Logger::get().info("Script Controller", "Compiling on Windows: " + cmd);
  
  char* cmd_buffer = _strdup(cmd.c_str());
    
  BOOL success = CreateProcessA(NULL, cmd_buffer, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
  free(cmd_buffer);
  
  if (success) {
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exit_code;
    GetExitCodeProcess(pi.hProcess, &exit_code);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if (exit_code == 0) {
      std::error_code ec;
      std::filesystem::rename(temp_so, so, ec);
      return (ec ? -1 : 0);
    };
  };
  
  return -1;

#else
  const char* command = compiler.c_str();
  const char* argv[] = {
    compiler.c_str(),
    "-rdynamic",
    "-shared", 
    "-fPIC",
    "-o", so.c_str(), 
    cpp.c_str(), 
    nullptr
  };

  pid_t pid = fork();
  if(pid == 0){
    execvp(command, const_cast<char* const*>(argv)); 
    Logger::get().erro("Script Controller", "Failed to exec g++");
    exit(-1);
  }
  else if(pid > 0){
    int status = 0; 
    
    waitpid(pid, &status, 0);
    
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
      UW::Logger::get().info("Script Controller", "successful compilation");
      return 0; 
    } 
    else { 
      UW::Logger::get().erro("Script Controller", "Compilation failed!");
      return -1; 
    };
  };

  UW::Logger::get().erro("Script Controller", "Failed to fork()");
  return -1;
#endif
};
