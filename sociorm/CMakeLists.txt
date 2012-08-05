cmake_minimum_required(VERSION 2.8)

set(SOCI_INCLUDE_DIRS 
	"${CMAKE_CURRENT_SOURCE_DIR}/../soci/src/core"
	"${CMAKE_CURRENT_SOURCE_DIR}/../soci/src/backends"
	)

include_directories(${SOCI_INCLUDE_DIRS} ${SQLITE3_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
add_definitions(-DSOCI_DLL)

file(GLOB detail sociorm/detail/*.hpp)
file(GLOB src src/*.hpp src/*.cpp sociorm/*.hpp)

source_group("Header Files\\detail" FILES ${detail})

add_library(sociorm SHARED ${src} ${detail})
add_library(sociorm_s STATIC ${src} ${detail})

add_executable(sociorm.examples.ex1 examples/ex1.cpp)
target_link_libraries(sociorm soci_core)
target_link_libraries(sociorm.examples.ex1 soci_core soci_sqlite3)
