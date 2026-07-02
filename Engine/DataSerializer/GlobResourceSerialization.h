// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>

#include "Renderer.h"
#include "Utils/Logger.h"
#include "ScriptShared/GlobResource.h"



namespace UW {
};



namespace UW {
struct GlobResourceRecord {
  std::string window_title = "";
  float Fixed_HZ = 16;
  unsigned int vsync = 0; 

  friend std::ostream& operator<<(std::ostream& os, const GlobResourceRecord& record);
  friend std::istream& operator>>(std::istream& is, GlobResourceRecord& record);
};



class GlobResourceSerialization {
public:
  GlobResourceSerialization() = default;
  ~GlobResourceSerialization() = default;

#ifndef PRODUCTION
  void saveAll();
#endif
  void loadAll();

private:
#ifndef PRODUCTION
  friend std::ostream& operator<<(std::ostream& os, const GlobResourceSerialization& record);
#endif
  friend std::istream& operator>>(std::istream& is, GlobResourceSerialization& record);
};
}; // namespace UW
