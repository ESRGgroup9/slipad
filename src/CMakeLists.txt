#cmake_minimum_required(VERSION 2.8)
##project( CCamera )
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
#set(CMAKE_CXX_COMPILER "/home/fernandes/buildroot/buildroot-2021.02.5/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-g++")
#project(slipad LANGUAGES CXX)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

##find_package( OpenCV REQUIRED )
##include_directories( ${OpenCV_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ ${CMAKE_CURRENT_SOURCE_DIR})
##include_directories( ${/home/fernandes/buildroot/buildroot-2021.02.5/output/host/arm-buildroot-linux-uclibcgnueabihf/} ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ ${CMAKE_CURRENT_SOURCE_DIR})
#include_directories( ${/home/fernandes/buildroot/buildroot-2021.02.5/output/host/usr/include} ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ ${CMAKE_CURRENT_SOURCE_DIR})
#include_directories(${OpenCV_INCLUDE_DIRS})
#find_package(OpenCV REQUIRED PATHS /home/fernandes/buildroot/buildroot-2021.02.5/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/share/OpenCV)
#add_executable( slipad CCamera.cpp )
##target_link_libraries( CCamera ${OpenCV_LIBS} )
#target_link_libraries(slipad ${OpenCV_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} -lrt -DWITH_FFMPEG=ON)

cmake_minimum_required(VERSION 3.16.3)
project( slipad LANGUAGES CXX)
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( OpenCV REQUIRED )
#find_package(OpenCV REQUIRED PATHS /home/fernandes/buildroot/buildroot-2021.02.5/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/share/OpenCV)

set(CMAKE_CXX_COMPILER "/home/fernandes/buildroot/buildroot-2021.02.5/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-g++")
set(CMAKE_CC_COMPILER "/home/fernandes/buildroot/buildroot-2021.02.5/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-gcc")

#set(CMAKE_CXX_COMPILER "/usr/bin/g++")
#set(CMAKE_CC_COMPILER "/usr/bin/gcc")

#set(CMAKE_CXX_FLAGS "-isystem /home/fernandes/buildroot/buildroot-2021.02.5/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/include")

#set(OpenCV_INCLUDE_DIRS "/home/fernandes/buildroot/buildroot-2021.02.5/output/build/opencv3-3.4.13/buildroot-build/include")
#set(OpenCV_INCLUDE_DIRS "/usr/include/opencv4/opencv2/")
#set(OpenCV_INCLUDE_DIRS "/home/fernandes/buildroot/buildroot-2021.02.5/output/build/opencv3-3.4.13/buildroot-build" "/home/fernandes/buildroot/buildroot-2021.02.5/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/include")
set(OpenCV_INCLUDE_DIRS "/usr/include/opencv2")
#set(LDFLAGS `pkg-config opencv --cflags --libs`)

#***********Libs***************
#set(OpenCV_LIBS "/home/fernandes/buildroot/buildroot-2021.02.5/output/build/opencv3-3.4.13/buildroot-build/lib/libopencv_videoio.so.3.4.13" "/home/fernandes/buildroot/buildroot-2021.02.5/output/build/opencv3-3.4.13/buildroot-build/lib/libopencv_videoio.so" "/home/fernandes/buildroot/buildroot-2021.02.5/output/build/opencv3-3.4.13/buildroot-build/lib/libopencv_videoio.so" "/home/fernandes/buildroot/buildroot-2021.02.5/output/build/opencv3-3.4.13/buildroot-build/lib/libopencv_videoio.so.3.4.13")
#set(OpenCV_LIBS libopencv_videoio.so.3.4.13 libopencv_videoio.so )

include_directories(include ${OpenCV_INCLUDE_DIRS})
include_directories(include ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ )

file(GLOB all_SRCS
   "${CMAKE_CURRENT_SOURCE_DIR}/CCamera.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/../inc/CCamera.h"
)

#***********output variables***************
message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
message(STATUS "OpenCV_LIBRARIES = ${OpenCV_LIBRARIES}")
message(STATUS "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")

add_executable( slipad testcamera.cpp ${all_SRCS})
target_link_libraries( slipad ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} -lrt -DWITH_FFMPEG=ON)


