# Help me I'am Under The Water
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

add_subdirectory(Utils)
if(NOT PRODUCTION)
  add_subdirectory(Editor)
endif()
add_subdirectory(Core)
add_subdirectory(App)
