# Engine
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

set(tests_src 
  Unit/Core/Camera/Camera.cpp
  Unit/Core/Camera/CameraController.cpp
  Unit/Utils/utils.cpp
  Unit/Utils/utilsProd.cpp
)

add_executable(unit_tests ${tests_src})

cmrc_add_resource_library(test_mesh_resources
  PREFIX "test_assets"
  test_data/box.obj
  test_data/character.fbx
  test_data/nested/house.obj
  test_data/ignored.txt
)

target_link_libraries(unit_tests PRIVATE 
  Core
  Utils
  test_mesh_resources
  gtest_main
  gmock_main
)

target_include_directories(unit_tests PRIVATE 
  ${CMAKE_SOURCE_DIR}/Engine/Core
  ${CMAKE_SOURCE_DIR}/Engine/Utils
)

include(GoogleTest)
gtest_discover_tests(unit_tests)