cmake_minimum_required(VERSION 2.6)
project (mocap2mav)
list( APPEND CMAKE_CXX_FLAGS "-std=c++11 -Wno-all -Wno-deprecated ${CMAKE_CXX_FLAGS} -g ")
# Set build directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

find_package(Eigen 3 QUIET)
if(NOT Eigen_FOUND)
    include(cmake/FindEigen3.cmake)
    #include(/usr/share/cmake-3.0/modules/FindEigen3.cmake)
    set(Eigen_INCLUDE_DIR ${EIGEN3_INCLUDE_DIR})
endif()

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(MODULES_SRC_DIR ${PROJECT_SOURCE_DIR}/modules)
set(PROJECT_LIB_DIR ${PROJECT_SOURCE_DIR}/libs)

include_directories(${Eigen_INCLUDE_DIR})
include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${PROJECT_LIB_DIR}")
include_directories("${MODULES_SRC_DIR}")

#Project modules
add_subdirectory(${MODULES_SRC_DIR}/Executioner)
add_subdirectory(${MODULES_SRC_DIR}/Automatic)
add_subdirectory(${MODULES_SRC_DIR}/Test)

#Project external libs
add_subdirectory(${PROJECT_LIB_DIR}/StateMachine)
add_subdirectory(${PROJECT_LIB_DIR}/miniPID)

