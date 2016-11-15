cmake_minimum_required(VERSION 2.6)
project (NatNetReceiver)
list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#Set QT
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network)

set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/listner.cpp ../src/MavState.cpp ../include/common/MavState.h)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("libs")
include_directories("include")

add_executable(sub ${PROJECT_SRCS})

#Set LCM
#set ( PROJECT_LINK_LIBS liblcm.so )
#link_directories( ~/usr/local/lib )
find_package(PkgConfig REQUIRED)
pkg_search_module(LCM REQUIRED lcm)


## Declare a C++ library
 add_library(NNRec
         libs/NatNetReceiver.cpp
         include/NatNetReceiver.h)

qt5_use_modules(sub Widgets Core Network)
qt5_use_modules(NNRec Widgets Core Network)
set_target_properties(NNRec PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(sub
        ${PROJECT_LINK_LIBS}
        NNRec
        Qt5::Core
        Qt5::Network
        Qt5::Widgets
        ${LCM_LIBRARIES}
        )

## Declare a C++ library
# add_library(position_control
#   src/lib/position_control.cpp
# )

## Specify libraries to link a library or executable target against

#target_link_libraries(position_control_node
#   position_control
#   ${catkin_LIBRARIES}
#        )
