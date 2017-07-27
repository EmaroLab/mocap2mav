cmake_minimum_required(VERSION 2.6)
project (Security)
#list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")

set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_INCLUDE_DIR}/common/MavState.cpp)

add_executable(secur ${PROJECT_SRCS})

#Set LCM
find_package(PkgConfig REQUIRED)
pkg_search_module(LCM REQUIRED lcm)

##Declare a C++ library
 add_library(SecLib
         libs/Security.cpp
         include/Security.h)

##qt5_use_modules(exec Widgets Core Network SerialPort)
##qt5_use_modules(Dis Widgets Core Network SerialPort)

target_link_libraries( secur
        ${PROJECT_LINK_LIBS}
		${LCM_LIBRARIES}
	    SecLib
	)

