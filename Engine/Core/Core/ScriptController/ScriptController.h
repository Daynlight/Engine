// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once

#include <string>
#include <filesystem>

#ifndef COMPILER_PATH
  #define COMPILER_PATH "g++"
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <dlfcn.h>
#include <sys/wait.h>
#endif
#include <fcntl.h>

#include "ScriptShared/GameObjectScriptInterface.h"
#include "Utils/Logger.h"
#include "Objects/ObjectManager.h"
#include "Utils/config.h"



namespace UW{
class GameObjectData;



class GameObjectScriptRecord{
  std::filesystem::file_time_type lastWriteTime{};
  std::string path = "";
  std::string so_file = "";
  std::string cpp_file = "";
  bool log_observe_lock = 1;
  bool compiling = 0;

#ifdef PRODUCTION
  bool module_initialized = false;
#endif

  void* script_handler = nullptr;

public:
  GameObjectScriptInterface* script = nullptr;
  bool script_on = true;

public:
  GameObjectScriptRecord(const std::string& path);
  ~GameObjectScriptRecord();

  GameObjectScriptRecord(const GameObjectScriptRecord& other);
  GameObjectScriptRecord& operator=(const GameObjectScriptRecord& other);
  GameObjectScriptRecord(GameObjectScriptRecord&& other) noexcept;
  GameObjectScriptRecord& operator=(GameObjectScriptRecord&& other) noexcept;
  void syncPointer(GameObjectData* data);

  void observe(GameObjectData* data);
  
  void onLoad(GameObjectData* data);
  void onUpdate(float delta_time);
  void onFixedUpdate(float fixed_delta_time);
  void onRender();
  void onDestroy();

  std::string getPath() const;

  int loadModule();
  void removeModule();

private:
  void initSharedFolder();
  bool checkLastWrite();
  void updateScript(GameObjectData* data);

  int compile();
  int compile_thread();

};
};
