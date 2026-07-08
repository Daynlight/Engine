# Help me I'am Under The Water
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

project(Core LANGUAGES CXX C)



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




if(PRODUCTION)
  file(GLOB_RECURSE GameDataSet CONFIGURE_DEPENDS "GameData/**")
  cmrc_add_resource_library(GameData ${GameDataSet})
endif()


file(GLOB_RECURSE ScriptSharedSet CONFIGURE_DEPENDS "ScriptShared/**")
cmrc_add_resource_library(ScriptShared ${ScriptSharedSet})



add_library(Core STATIC ${src})



if(PRODUCTION)
  target_compile_definitions(Core PRIVATE PRODUCTION)
  file(GLOB_RECURSE PROD_SCRIPTS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../Scripts/*.cpp")
  target_sources(Core PRIVATE ${PROD_SCRIPTS})
endif()

target_link_libraries(Core CWindow Utils)


if(PRODUCTION)
  target_link_libraries(Core GameData)
else()
  target_link_libraries(Core ScriptShared)
endif()


target_include_directories(Core PUBLIC "Core/" "../ScriptShared")


if(PRODUCTION)
  target_compile_definitions(Core PRIVATE PRODUCTION)
endif()


target_compile_definitions(Core PRIVATE COMPILER_PATH="${CMAKE_CXX_COMPILER}")
message(STATUS "ScriptController uses CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
message(STATUS "ScriptController uses COMPILER_PATH definition=${CMAKE_CXX_COMPILER}")


# if(SANDBOX_SCRIPTS)
#   target_compile_definitions(Core PRIVATE SANDBOX_SCRIPTS)
# endif()
