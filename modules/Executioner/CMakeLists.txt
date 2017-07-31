cmake_minimum_required(VERSION 2.6)
project (Executioner CXX)
#list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SOURCE_DIR}/Parser.cpp)

include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("include")
include_directories("dir")

add_executable(exec ${PROJECT_SRCS} ${PROJECT_INCLUDE_DIR}/utils/TimeHelpers.hpp)

find_package(PkgConfig REQUIRED)
pkg_search_module(LCM REQUIRED lcm)

target_link_libraries(exec
        mocap
        ${LCM_LIBRARIES}
	)


