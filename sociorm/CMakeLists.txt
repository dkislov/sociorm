cmake_minimum_required(VERSION 2.8)

set(SOCI_INCLUDE_DIRS 
	"${CMAKE_CURRENT_SOURCE_DIR}/../soci/src/core"
	"${CMAKE_CURRENT_SOURCE_DIR}/../soci/src/backends"
	)

include_directories(${SOCI_INCLUDE_DIRS} ${SQLITE3_INCLUDE_DIR})
add_definitions(-DSOCI_DLL)

file(GLOB src *.hpp *.cpp)

add_executable(sociorm ${src})
target_link_libraries(sociorm soci_core soci_sqlite3)
