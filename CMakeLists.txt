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
option(CWD "Current Resources Dir" "")



if(NOT DEFINED VCPKG_INSTALLED_DIR)
  set(VCPKG_INSTALLED_DIR "${CMAKE_SOURCE_DIR}/vendor/x64-linux")
endif()
set(ENGINE_VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor/x64-linux")

list(APPEND CMAKE_PREFIX_PATH "${VCPKG_INSTALLED_DIR}")

find_library(
  CWINDOW_LIBRARY
  NAMES CWindow
  PATHS
    "${VCPKG_TRIPLET_DIR}/lib"
    "${VCPKG_TRIPLET_DIR}/debug/lib"
  NO_DEFAULT_PATH
  REQUIRED
)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(CMakeRC CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(GTest CONFIG REQUIRED)


set(ENGINE_SRC "${CMAKE_BINARY_DIR}/Engine")
if(DEFINED ENV{XDG_DATA_HOME})
  set(ENGINE_SRC_DIR "$ENV{XDG_DATA_HOME}/Engine")
else()
  set(ENGINE_SRC_DIR "$ENV{HOME}/.local/share/Engine")
endif()

set(ENGINE_SRC_DEST "${ENGINE_SRC_DIR}/Engine")

if(NOT PRODUCTION)
  file(REMOVE_RECURSE "${ENGINE_SRC_DEST}")
  file(MAKE_DIRECTORY "${ENGINE_SRC_DEST}")

  file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Engine"
       DESTINATION "${ENGINE_SRC_DEST}"
       PATTERN "Examples" EXCLUDE)

  file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Tests"
       DESTINATION "${ENGINE_SRC_DEST}"
       PATTERN "Examples" EXCLUDE)

  file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Benchmark"
       DESTINATION "${ENGINE_SRC_DEST}"
       PATTERN "Examples" EXCLUDE)

  file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/vendor"
       DESTINATION "${ENGINE_SRC_DEST}"
       PATTERN "Examples" EXCLUDE)

  file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
       DESTINATION "${ENGINE_SRC_DEST}")
endif()

message(STATUS "CWD = ${CWD}")
file(GLOB_RECURSE GameDataSet CONFIGURE_DEPENDS "${CWD}GameData/**")
cmrc_add_resource_library(GameData WHENCE "${CWD}" ${GameDataSet})
message(STATUS "============ GameData Resources Found: =============")
foreach(res ${GameDataSet})
  message(STATUS "* " ${res})
endforeach()
message(STATUS "================ GameData Resources ================")

file(GLOB_RECURSE PROD_SCRIPTS CONFIGURE_DEPENDS "${CWD}Scripts/*.cpp")
message(STATUS "================== Scripts Found: ==================")
foreach(res ${PROD_SCRIPTS})
  message(STATUS "* " ${res})
endforeach()
message(STATUS "====================== Scripts =====================")

add_subdirectory(Engine)

enable_testing()
add_subdirectory(Tests)

add_subdirectory(Benchmark)


install(TARGETS Engine
  DESTINATION bin
)
