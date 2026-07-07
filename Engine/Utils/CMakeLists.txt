# Help me I'am Under The Water
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

project(Utils LANGUAGES CXX C)


set(src
  "Utils/Logger.cpp"
)


add_library(Utils STATIC ${src})
target_link_libraries(Utils CWindow)

target_include_directories(Utils PUBLIC "." "../ScriptShared" "../Config")


if(PRODUCTION)
  target_compile_definitions(Utils PRIVATE PRODUCTION)
endif()
