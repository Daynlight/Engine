// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include <string>
#include <vector>

#include "GameObjectData.h"


namespace UW {
class IObjectManager {
public:
  virtual void emplace_back(const std::string& name) = 0;
  virtual void erase(const std::string& name) = 0;
  virtual GameObjectData* getGameObjectData(const std::string& name) = 0;
  virtual void addScript(const std::string& object_name, const std::string& path) = 0;
  virtual void removeScript(const std::string& object_name, const std::string& path) = 0;
  virtual void saveRuntime(const std::string& object_name) = 0;

  virtual void emplace_backObjectScript(const std::string& name) = 0;
  virtual void eraseObjectScript(const std::string& name) = 0;
  virtual GameObjectData* getGameObjectDataObjectScript(const std::string& name) = 0;
  virtual void addScriptObjectScript(const std::string& object_name, const std::string& path) = 0;
  virtual void removeScriptObjectScript(const std::string& object_name, const std::string& path) = 0;
  virtual void saveRuntimeObjectScript(const std::string& object_name) = 0;
};
};
