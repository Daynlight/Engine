// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#ifdef PRODUCTION

#include <string>
#include <unordered_map>
#include <functional>
#include <cstdio>
#include "GameObjectScriptInterface.h"
#include "ILogger.h"



namespace Engine::ScriptShared {
  class GameObjectScriptInterface;

  using ScriptFactory = std::function<GameObjectScriptInterface*()>;

  class ScriptRegistry {
  private:
    std::unordered_map<std::string, ScriptFactory> factories;

    ScriptRegistry() = default;

public:
    static ScriptRegistry& get() {
      static ScriptRegistry instance;
      return instance;
    };

    void registerScript(const std::string& name, ScriptFactory factory) {
      printf("Reg: %s\n", name.c_str());
      factories[name] = factory;
    };

    GameObjectScriptInterface* createScript(const std::string& name) {
      auto it = factories.find(name);
      if (it != factories.end()) return it->second();
      return nullptr;
    };
  };
};

#endif



#ifdef PRODUCTION
  #define REGISTER_SCRIPT_INTERNAL(RegKey, ScriptClassName) \
    namespace { \
      struct ScriptRegisterer_##ScriptClassName { \
        ScriptRegisterer_##ScriptClassName() { \
          Engine::ScriptShared::ScriptRegistry::get().registerScript(RegKey, []() -> Engine::ScriptShared::GameObjectScriptInterface* { \
            return new Engine::ScriptClassName(); \
          }); \
        } \
      }; \
      static ScriptRegisterer_##ScriptClassName global_ScriptRegisterer_##ScriptClassName; \
    }

  #define REGISTER_SCRIPT(RegKey, ScriptClassName) REGISTER_SCRIPT_INTERNAL(RegKey, ScriptClassName)
#else
  #define REGISTER_SCRIPT(RegKey, ScriptClassName) \
    extern "C" Engine::ScriptShared::GameObjectScriptInterface* SCRIPT_API GetScript() { \
      Engine::ScriptClassName* script = new Engine::ScriptClassName(); \
      return (Engine::ScriptShared::GameObjectScriptInterface*)script; \
    }; \
       \
       \
       \
    extern "C" void SCRIPT_API DeleteScript(Engine::ScriptShared::GameObjectScriptInterface* script) { \
      Engine::ScriptClassName* temp_script = (Engine::ScriptClassName*)script; \
      delete temp_script; \
    };
#endif