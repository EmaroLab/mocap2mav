cmake_minimum_required(VERSION 2.6)
project (Executioner)
list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#Set QT
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network)

set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/main.cpp)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("libs")
include_directories("include")

add_executable(exec ${PROJECT_SRCS})

#Set LCM
set ( PROJECT_LINK_LIBS liblcm.so )
link_directories( ~/usr/local/lib )

##Declare a C++ library
 add_library(Execlib
         libs/Executioner.cpp
         include/Executioner.h)

##qt5_use_modules(exec Widgets Core Network SerialPort)
##qt5_use_modules(Dis Widgets Core Network SerialPort)

target_link_libraries(exec
        ${PROJECT_LINK_LIBS}
        Qt5::Core
        Qt5::Network
        Qt5::Widgets
        Execlib
        AndOrLib
	)

add_subdirectory(libs/AndOr)