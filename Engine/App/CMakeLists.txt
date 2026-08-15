# Engine
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

project(Engine LANGUAGES CXX C)
project(App LANGUAGES CXX C)


set(src
  "App/main.cpp"
  "App/App.cpp"
)


add_executable(Engine ${src})
target_link_libraries(Engine CoreDev)
target_include_directories(Engine PUBLIC "App/")
target_link_libraries(Engine Editor)


add_executable(App ${src})
target_link_libraries(App PRIVATE "-Wl,--whole-archive" Core "-Wl,--no-whole-archive")
target_include_directories(App PUBLIC "App/")
target_compile_definitions(App PRIVATE PRODUCTION)
