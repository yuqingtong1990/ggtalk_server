cmake_minimum_required (VERSION 3.4)
project(all)

#set( CMAKE_BUILD_TYPE Debug )
set( CMAKE_BUILD_TYPE Release)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread" )

include_directories(${PROJECT_SOURCE_DIR}/)
include_directories(${PROJECT_SOURCE_DIR}/base)
include_directories(${PROJECT_SOURCE_DIR}/mysql)

link_directories(${PROJECT_BINARY_DIR}/libs /usr/local/lib /usr/lib)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/libs)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(base)
add_subdirectory(evpp)
add_subdirectory(server/cdn_server)
add_subdirectory(server/db_server)



