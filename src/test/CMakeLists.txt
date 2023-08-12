set(CMAKE_C_STANDARD 11)

project("jhid3-test" C)

# add_compile_options(-W -Wall -Werror -O3)

set(SOURCES 
  main.c
  )


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../../bin)

include_directories(../lib)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} m jhid3v2)
