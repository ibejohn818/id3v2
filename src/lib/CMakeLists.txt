set(CMAKE_C_STANDARD 11)

project("id3v2" C)

# add_compile_options(-W -Wall -Werror -O3)

include(FindPkgConfig)
pkg_search_module(LIBSODIUM OPTIONAL libsodium)



set(SOURCES
#  ${id3v2lib_SOURCE_DIR}/src/include/id3v2/types.h
  synch.c
  helpers.c
  id3v2.c
  )

set(VERSION "0.1.0")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../../bin)



# add_compile_options(-fPIC)

include_directories(
#  ${id3v2lib_SOURCE_DIR}/src/include/id3v2
  ${id3v2lib_SOURCE_DIR}/src/include
  ${LIBSODIUM_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBSODIUM_LIBRARIES}) 

configure_file(id3v2.pc.in ${CMAKE_BINARY_DIR}/id3v2.pc
)

set(CMAKE_INSTALL_MODE "REL_SYMLINK")
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${id3v2lib_SOURCE_DIR}/src/include/id3v2.h DESTINATION include)
install(DIRECTORY ${id3v2lib_SOURCE_DIR}/src/include/id3v2 DESTINATION include)

execute_process(COMMAND which apt
                RESULT_VARIABLE HAS_APT)

execute_process(COMMAND which rpm
                RESULT_VARIABLE HAS_RPM)


message("-- CMAKE_SYSTEM_INFO_FILE: ${CMAKE_SYSTEM_INFO_FILE}")
message("-- CMAKE_SYSTEM_NAME:      ${CMAKE_SYSTEM_NAME}")
message("-- CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message("-- CMAKE_SYSTEM:           ${CMAKE_SYSTEM}")
message("-- CMAKE_OS_NAME:          ${CMAKE_OS_NAME}")
message("-- HAS_APT:                ${HAS_APT}")
message("-- HAS_RPM:                ${HAS_RPM}")

set(PKG_GENERATORS 
  "TGZ"
  "STGZ"
)

if(${HAS_APT} EQUAL 0)
  list(APPEND PKG_GENERATORS "DEB")
endif()

if(${HAS_RPM} EQUAL 0)
  list(APPEND PKG_GENERATORS "RPM")
endif()

# create install packages
# set(ID3V2_GENERATORS "RPM" "TGZ" "STGZ")
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_GENERATOR ${PKG_GENERATORS})
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "John Hardy")
set(CPACK_PACKAGE_VENDOR "johnchardy.com")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_PACKAGE_DIRECTORY "${id3v2lib_SOURCE_DIR}/dist/")
include(CPack)


get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
