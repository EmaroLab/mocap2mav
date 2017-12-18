cmake_minimum_required(VERSION 2.6)
project (Automatic CXX)
#list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(include)
include_directories(include/Command/)

file(GLOB_RECURSE COMM include/Command/*.hpp)
file(GLOB_RECURSE SRCS src/*.cpp)
file(GLOB_RECURSE LAND include/StatesClasses.hpp)

##Declare a C++ library
 add_library(mocap
		 ${SRCS}
		 ${COMM}
		 ${LAND}
		 ${PROJECT_INCLUDE_DIR}/common/MavState.cpp)

##Workaround for rude...so rude
target_link_libraries(mocap StateMachine miniPID /usr/local/lib/librudeconfig.so)


