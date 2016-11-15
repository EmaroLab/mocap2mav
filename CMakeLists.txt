cmake_minimum_required(VERSION 2.6)
project (mocap2mav)


# Set build directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Eigen 3 QUIET)
if(NOT Eigen_FOUND)
    include(/usr/share/cmake-3.0/Modules/FindEigen3.cmake)
    set(Eigen_INCLUDE_DIR ${EIGEN3_INCLUDE_DIR})
endif()
include_directories(${Eigen_INCLUDE_DIR})

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SRC_DIR ${PROJECT_SOURCE_DIR}/src)





include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${PROJECT_SRC_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

add_subdirectory(NatNetReceiver)
add_subdirectory(Dispatcher)
add_subdirectory(Executioner)
add_subdirectory(Automatic)
add_subdirectory(Security)
