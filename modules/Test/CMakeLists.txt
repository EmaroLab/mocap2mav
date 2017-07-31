cmake_minimum_required(VERSION 2.6)
project (TestModule CXX)
#list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(PROJECT_SRCS
        src/maintest.cpp ${PROJECT_INCLUDE_DIR}/common/MavState.cpp)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("include")

#add_executable(testM ${PROJECT_SRCS})
add_executable(mocapTest src/mocapTest.cpp ${PROJECT_INCLUDE_DIR}/common/MavState.cpp)
add_executable(dummypose src/dummyPose.cpp ../../include/utils/TimeHelpers.hpp)

find_package(PkgConfig REQUIRED)
pkg_search_module(LCM REQUIRED lcm)

#target_link_libraries(Automlib StateMachine)

target_link_libraries(dummypose  ${LCM_LIBRARIES})
target_link_libraries(mocapTest  mocap)