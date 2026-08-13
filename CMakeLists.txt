# Engine
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


option(PRODUCTION "Enable production build flags" OFF)
option(SANDBOX_SCRIPTS "Enable script sandboxing build flags" OFF)


execute_process(
  COMMAND git submodule update --init --recursive
)

add_subdirectory(vendor/CWindow)
add_subdirectory(vendor/cmrc)
add_subdirectory(vendor/googletest vendor/googletest EXCLUDE_FROM_ALL)


set(ENGINE_SRC "${CMAKE_BINARY_DIR}/Engine")
if(DEFINED ENV{XDG_DATA_HOME})
  set(ENGINE_SRC_DIR "$ENV{XDG_DATA_HOME}/Engine")
else()
  set(ENGINE_SRC_DIR "$ENV{HOME}/.local/share/Engine")
endif()

set(ENGINE_SRC_DEST "${ENGINE_SRC_DIR}/Engine")

file(GLOB_RECURSE EngineDataSet CONFIGURE_DEPENDS "Engine/**" "CMakeLists.txt" "vendor/**")
list(FILTER EngineDataSet EXCLUDE REGEX ".*/Examples/.*")

if(NOT PRODUCTION)
  file(REMOVE_RECURSE "${ENGINE_SRC_DEST}")
  
  foreach(file_path IN LISTS EngineDataSet)
    file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}" "${file_path}")
    
    set(dest_file "${ENGINE_SRC_DEST}/${rel_path}")
    get_filename_component(dest_dir "${dest_file}" DIRECTORY)
    
    file(MAKE_DIRECTORY "${dest_dir}")
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E copy "${file_path}" "${dest_file}"
      RESULT_VARIABLE copy_result
    )
  endforeach()
endif()
  

file(GLOB_RECURSE GameDataSet CONFIGURE_DEPENDS "GameData/**")
cmrc_add_resource_library(GameData ${GameDataSet})

file(GLOB_RECURSE PROD_SCRIPTS CONFIGURE_DEPENDS "Scripts/*.cpp")

add_subdirectory(Engine)

enable_testing()
add_subdirectory(Tests)


install(TARGETS Engine
  DESTINATION bin
)
