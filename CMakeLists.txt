cmake_minimum_required(VERSION 2.8)

set(SQLITE3_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../sqlite3.install")
set(SQLITE3_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/../sqlite3.install/sqlite3_s.lib")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(soci/src)
add_subdirectory(sociorm)
