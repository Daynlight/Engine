# Engine
# Copyright 2026 Daynlight
# Licensed under the GNU General, Version 3.0.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

project(Utils LANGUAGES CXX C)
project(UtilsDev LANGUAGES CXX C)


set(src
  "Utils/Logger.cpp"
)


add_library(Utils STATIC ${src})
target_link_libraries(Utils "${CWINDOW_LIBRARY}" glfw glad::glad)
target_include_directories(Utils PUBLIC "." "../ScriptShared" "${ENGINE_VENDOR_DIR}/include/")
target_compile_definitions(Utils PRIVATE PRODUCTION)

add_library(UtilsDev STATIC ${src})
target_link_libraries(UtilsDev "${CWINDOW_LIBRARY}" glfw glad::glad)
target_include_directories(UtilsDev PUBLIC "." "../ScriptShared" "${ENGINE_VENDOR_DIR}/include/")
