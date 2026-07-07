# Help me I'am Under The Water
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

project(App LANGUAGES CXX C)


set(src
  "App/main.cpp"
  "App/App.cpp"
)


add_executable(App ${src})
target_link_libraries(App Core)

target_include_directories(App PUBLIC "App/" "../Config")


if(PRODUCTION)
  target_compile_definitions(App PRIVATE PRODUCTION)
endif()
