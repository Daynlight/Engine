# Engine
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

project(Core LANGUAGES CXX C)
project(CoreDev LANGUAGES CXX C)


set(src
  "Core/Core.cpp"
  "Core/Scene.cpp"
  "Core/Camera/Camera.cpp"
  "Core/Resources/Resources.cpp"
  "Core/Resources/Lights/Lights.cpp"
  "Core/Resources/Materials/Materials.cpp"
  "Core/Objects/GameObject.cpp"
  "Core/Objects/ObjectManager.cpp"
  "Core/DataSerializer/GlobResourceSerialization.cpp"
  "Core/ScriptController/ScriptController.cpp"
  "Core/DataSerializer/DataSerializer.cpp"
  "Core/DataSerializer/MeshSerialization.cpp"
  "Core/DataSerializer/ObjectsSerialization.cpp"
  "Core/DataSerializer/MaterialsSerialization.cpp"
  "Core/DataSerializer/LightsSerialization.cpp"
  "Core/DataSerializer/ShaderSerialization.cpp"
  "Core/DataSerializer/ScriptSerialization.cpp"
  "Core/DataSerializer/TextureSerialization.cpp"
)


add_library(Core STATIC ${src} ${PROD_SCRIPTS})
target_link_libraries(Core CWindow Utils)
target_link_libraries(Core GameData)
target_include_directories(Core PUBLIC "Core/" "../ScriptShared")
target_compile_definitions(Core PRIVATE PRODUCTION)

add_library(CoreDev STATIC ${src})
target_link_libraries(CoreDev CWindow UtilsDev)
target_link_libraries(CoreDev ScriptShared)
target_include_directories(CoreDev PUBLIC "Core/" "../ScriptShared")
target_compile_definitions(CoreDev PRIVATE COMPILER_PATH="${CMAKE_CXX_COMPILER}")
message(STATUS "ScriptController uses CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
message(STATUS "ScriptController uses COMPILER_PATH definition=${CMAKE_CXX_COMPILER}")




# if(SANDBOX_SCRIPTS)
#   target_compile_definitions(Core PRIVATE SANDBOX_SCRIPTS)
# endif()
