# Help me I'am Under The Water
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

project(Editor LANGUAGES CXX C)


set(src
  "Editor/Editor.cpp"
  "Editor/UI/UI.cpp"
  "Editor/UI/UI_Logs.cpp"
  "Editor/UI/UI_Materials.cpp"
  "Editor/UI/UI_Shaders.cpp"
  "Editor/UI/UI_Scripts.cpp"
  "Editor/UI/UI_Objects.cpp"
  "Editor/UI/UI_Lights.cpp"
  "Editor/UI/UI_AssetLoader.cpp"
  "Editor/UI/UI_ShaderEditors.cpp"
  "Editor/UI/UI_ScriptEditor.cpp"
  "Editor/UI/UI_Info.cpp"
)

add_library(Editor STATIC ${src})
target_link_libraries(Editor CWindow Utils)

target_include_directories(Editor PUBLIC "Editor/" "../Core/Core" "../ScriptShared")

if(PRODUCTION)
  target_compile_definitions(Editor PRIVATE PRODUCTION)
endif()
