# Help me I'am Under The Water
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(PRODUCTION "Enable production build flags" OFF)
# option(SANDBOX_SCRIPTS "Enable script sandboxing build flags" ON)



include(FetchContent)


FetchContent_Declare(CWindow
  GIT_REPOSITORY  https://github.com/Daynlight/CWindow.git
  GIT_TAG under_water
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(CWindow)


if(PRODUCTION)
  FetchContent_Declare(cmrc
    GIT_REPOSITORY  https://github.com/vector-of-bool/cmrc.git
    GIT_TAG master
    GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(cmrc)

  file(GLOB_RECURSE GameDataSet CONFIGURE_DEPENDS "GameData/**")
  cmrc_add_resource_library(GameData ${GameDataSet})
endif()

add_subdirectory(Engine)


if(PRODUCTION)
  target_compile_definitions(Engine PRIVATE PRODUCTION)
endif()


target_compile_definitions(Engine PRIVATE COMPILER_PATH="${CMAKE_CXX_COMPILER}")
message(STATUS "ScriptController uses CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
message(STATUS "ScriptController uses COMPILER_PATH definition=${CMAKE_CXX_COMPILER}")


# if(SANDBOX_SCRIPTS)
#   target_compile_definitions(Engine PRIVATE SANDBOX_SCRIPTS)
# endif()
