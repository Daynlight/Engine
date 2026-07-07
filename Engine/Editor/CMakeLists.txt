# Help me I'am Under The Water
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

project(Editor LANGUAGES CXX C)


set(src
  "UI/UI.cpp"
  "UI/UI_Logs.cpp"
  "UI/UI_Materials.cpp"
  "UI/UI_Shaders.cpp"
  "UI/UI_Scripts.cpp"
  "UI/UI_Objects.cpp"
  "UI/UI_Lights.cpp"
  "UI/UI_AssetLoader.cpp"
  "UI/UI_ShaderEditors.cpp"
  "UI/UI_ScriptEditor.cpp"
  "UI/UI_Info.cpp"
)

add_library(Editor STATIC ${src})
target_link_libraries(Editor CWindow Utils)

target_include_directories(Editor PUBLIC "." "../Config" "../Core/Core" "../ScriptShared")