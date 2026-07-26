# Engine
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


option(PRODUCTION "Enable production build flags" OFF)
option(SANDBOX_SCRIPTS "Enable script sandboxing build flags" OFF)


execute_process(
  COMMAND git submodule update --init --recursive
)

add_subdirectory(vendor/CWindow)
add_subdirectory(vendor/cmrc)


set(ENGINE_ZIP "${CMAKE_BINARY_DIR}/engine_data.zip")
if(DEFINED ENV{XDG_DATA_HOME})
  set(ENGINE_ZIP_DIR "$ENV{XDG_DATA_HOME}/Engine")
else()
  set(ENGINE_ZIP_DIR "$ENV{HOME}/.local/share/Engine")
endif()
file(MAKE_DIRECTORY "${ENGINE_ZIP_DIR}")
set(ENGINE_ZIP_DEST "${ENGINE_ZIP_DIR}/engine_data.zip")

file(GLOB_RECURSE EngineSet CONFIGURE_DEPENDS "Engine/**" "CMakeLists.txt" "vendor/**")
list(FILTER EngineSet EXCLUDE REGEX ".*/Examples/.*")
list(FILTER EngineSet EXCLUDE REGEX ".*engine_data\\.zip$")

if(NOT PRODUCTION)
  file(ARCHIVE_CREATE
    OUTPUT "${ENGINE_ZIP}"
    PATHS ${EngineSet}
    FORMAT zip
  )

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy "${ENGINE_ZIP}" "${ENGINE_ZIP_DEST}"
    RESULT_VARIABLE copy_result
    COMMAND_ECHO STDOUT
  )
endif()
  

file(GLOB_RECURSE GameDataSet CONFIGURE_DEPENDS "GameData/**")
cmrc_add_resource_library(GameData ${GameDataSet})

file(GLOB_RECURSE PROD_SCRIPTS CONFIGURE_DEPENDS "Scripts/*.cpp")

add_subdirectory(Engine)


install(TARGETS Engine
  DESTINATION bin
)
