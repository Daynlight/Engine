# Help me I'am Under The Water
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


option(PRODUCTION "Enable production build flags" OFF)
option(SANDBOX_SCRIPTS "Enable script sandboxing build flags" OFF)


include(FetchContent)

FetchContent_Declare(CWindow
  GIT_REPOSITORY  https://github.com/Daynlight/CWindow.git
  GIT_TAG engine
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(CWindow)

FetchContent_Declare(cmrc
  GIT_REPOSITORY  https://github.com/vector-of-bool/cmrc.git
  GIT_TAG master
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(cmrc)


file(GLOB_RECURSE GameDataSet CONFIGURE_DEPENDS "GameData/**")
cmrc_add_resource_library(GameData ${GameDataSet})

file(GLOB_RECURSE PROD_SCRIPTS CONFIGURE_DEPENDS "Scripts/*.cpp")


add_subdirectory(Engine)
