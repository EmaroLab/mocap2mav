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
${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SRC_DIR}/MavState.cpp include/parameters.h include/Command/Command.hpp include/Command/Move.hpp include/Command/TakeOff.hpp include/Command/Rotate.hpp include/Command/Land.hpp)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("include")

add_executable(autom ${PROJECT_SRCS})

#Set LCM
#set ( PROJECT_LINK_LIBS liblcm.so )
#link_directories( ~/usr/local/lib )

find_package(PkgConfig REQUIRED)
pkg_search_module(LCM REQUIRED lcm)

##Declare a C++ library
 add_library(Automlib
         libs/Automatic.cpp
         include/Automatic.h
		 ${PROJECT_SRC_DIR}/MavState.cpp
		 ${PROJECT_INCLUDE_DIR}/common/MavState.h
		 libs/Lander.cpp
		 include/Lander/Lander.h
		 include/Lander/StatesClasses.hpp
		 libs/StateClasses.cpp)
		 #${PROJECT_LIB_DIR}/StateMachine/include/Machine.h
		 #${PROJECT_LIB_DIR}/StateMachine/include/AbstractState.h
		 #${PROJECT_LIB_DIR}/StateMachine/src/Machine.cpp)

target_link_libraries(Automlib StateMachine miniPID)
##qt5_use_modules(exec Widgets Core Network SerialPort)
##qt5_use_modules(Dis Widgets Core Network SerialPort)
target_link_libraries( autom
	    Automlib
		${LCM_LIBRARIES}
	)

