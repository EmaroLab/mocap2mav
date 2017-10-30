cmake_minimum_required(VERSION 2.6)
project (Automatic CXX)
#list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#Set QT
##find_package(Qt5Widgets REQUIRED)
#find_package(Qt5Network)

set(PROJECT_SRCS
src/main.cpp)

include_directories("${PROJECT_INCLUDE_DIR}")

add_executable(autom ${PROJECT_SRCS})

find_package(PkgConfig REQUIRED)
pkg_search_module(LCM REQUIRED lcm)

target_link_libraries( autom
	    mocap
		${LCM_LIBRARIES}
		/usr/local/lib/librudeconfig.so
	)

