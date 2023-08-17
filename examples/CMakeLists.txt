project("id3v2-examples" C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(../src/lib)

add_executable(edit-tag edit_tag.c)
target_link_libraries(edit-tag id3v2)

add_executable(scan-dir scan_dir.c)
target_link_libraries(scan-dir id3v2)
