cmake_minimum_required (VERSION 3.4)
project(cdn_server)
set(CMAKE_CXX_STANDARD 11)
file(GLOB CHATSRV_SRCS *.cpp */*.cpp *.h */*.h  *.cc */*.cc)
link_libraries(base_static evpp_static jsoncpp)
add_executable(cdn_server ${CHATSRV_SRCS})