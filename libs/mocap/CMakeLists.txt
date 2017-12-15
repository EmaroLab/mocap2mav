cmake_minimum_required(VERSION 2.6)
project (Automatic CXX)
#list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE COMM Automatic/include/Command/*.hpp)
file(GLOB_RECURSE AUTO Automatic/libs/*)
file(GLOB_RECURSE LAND Automatic/include/Lander/*.cpp Automatic/include/Lander/*.hpp)
file(GLOB_RECURSE PARA Parameters/Parameters.cpp)

include_directories(Automatic/include)
include_directories(Executioner/include)
include_directories(Parameters/include)

##Declare a C++ library
 add_library(mocap
		 ${PARA}
		 ${COMM}
		 ${AUTO}
		 ${LAND}
		 ${PROJECT_INCLUDE_DIR}/common/MavState.cpp
		 Executioner/Executioner.cpp)
target_link_libraries(mocap StateMachine miniPID)


