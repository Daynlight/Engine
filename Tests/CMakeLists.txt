# Engine
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

set(tests_src 
  Unit/Core/Camera/Camera.cpp
)

add_executable(unit_tests ${tests_src})

target_link_libraries(unit_tests PRIVATE 
  Core 
  gtest_main
  gmock_main
)

target_include_directories(unit_tests PRIVATE 
  ${CMAKE_SOURCE_DIR}/Engine/Core
)

include(GoogleTest)
gtest_discover_tests(unit_tests)