cmake_minimum_required(VERSION 3.20)
project(id3v2lib C)

set(CMAKE_C_STANDARD 11)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "SHARED?" ON)

add_subdirectory("src/lib")
add_subdirectory("src/test")
add_subdirectory("examples")

if( EXISTS "${PROJECT_BINARY_DIR}/compile_commands.json" )
  execute_process( COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${PROJECT_BINARY_DIR}/compile_commands.json
      ${PROJECT_SOURCE_DIR}/compile_commands.json
  )
endif()
