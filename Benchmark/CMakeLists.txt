# Engine
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)
project(Benchmark LANGUAGES CXX)

set(src 
  main.cpp
)

add_executable(Benchmark ${src})

target_link_libraries(Benchmark PRIVATE 
  Utils
  fmt
)

target_include_directories(unit_tests PRIVATE 
  ${CMAKE_SOURCE_DIR}/Engine/Core
  ${CMAKE_SOURCE_DIR}/Engine/Utils
)
