cmake_minimum_required(VERSION 2.8)
#project( CCamera )
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
set(CMAKE_CXX_COMPILER "/home/fernandes/buildroot/buildroot-2021.02.5/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-g++")
project(slipad LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ ${CMAKE_CURRENT_SOURCE_DIR})
find_package(OpenCV REQUIRED PATHS /home/fernandes/buildroot/buildroot-2021.02.5/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/share/OpenCV)
add_executable( CCamera CCamera.cpp )
target_link_libraries( CCamera ${OpenCV_LIBS} )
