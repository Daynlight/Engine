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



set(ENGINE_ZIP "${CMAKE_BINARY_DIR}/engine_data.zip")
if(DEFINED ENV{XDG_DATA_HOME})
  set(ENGINE_ZIP_DIR "$ENV{XDG_DATA_HOME}/Engine")
else()
  set(ENGINE_ZIP_DIR "$ENV{HOME}/.local/share/Engine")
endif()
file(MAKE_DIRECTORY "${ENGINE_ZIP_DIR}")
set(ENGINE_ZIP_DEST "${ENGINE_ZIP_DIR}/engine_data.zip")


file(GLOB_RECURSE EngineSet CONFIGURE_DEPENDS "Engine/**" "CMakeLists.txt" "${CMAKE_BINARY_DIR}/build-prod/**" "bin/**")
list(FILTER EngineSet EXCLUDE REGEX ".*/Examples/.*")
list(FILTER EngineSet EXCLUDE REGEX ".*engine_data\\.zip$")
list(FILTER EngineSet EXCLUDE REGEX ".*/CMakeCache\\.txt$")
list(FILTER EngineSet EXCLUDE REGEX ".*/CMakeFiles/.*")

if(NOT EXISTS "${ENGINE_ZIP_DEST}")
  if(NOT IS_PROD_BUILD)
    message(STATUS "Synchronously configuring production build...")
    execute_process(
      COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}/build-prod -G "${CMAKE_GENERATOR}" -DIS_PROD_BUILD=ON
      RESULT_VARIABLE prod_config_result
      COMMAND_ECHO STDOUT
    )

    if(prod_config_result EQUAL 0)
      message(STATUS "Synchronously building production App...")
      execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/build-prod --target App
        RESULT_VARIABLE prod_build_result
      )
      if(NOT prod_build_result EQUAL 0)
        message(FATAL_ERROR "Production build failed. Cannot continue configuration.")
      endif()
    else()
      message(FATAL_ERROR "Production configuration failed. Cannot continue.")
    endif()
    
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
endif()
  


file(GLOB_RECURSE GameDataSet CONFIGURE_DEPENDS "GameData/**")
cmrc_add_resource_library(GameData ${GameDataSet})

file(GLOB_RECURSE PROD_SCRIPTS CONFIGURE_DEPENDS "Scripts/*.cpp")

add_subdirectory(Engine)
target_compile_definitions(Editor PRIVATE ENGINE_DATA_ZIP="${ENGINE_ZIP_DEST}")
target_compile_definitions(Editor PRIVATE GENERATOR="${CMAKE_GENERATOR}")



install(TARGETS AppDev
  DESTINATION bin
)
