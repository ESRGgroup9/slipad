cmake_minimum_required(VERSION 3.16.3)
project( slipad LANGUAGES CXX)

find_package( OpenCV REQUIRED )

#set(CMAKE_CXX_COMPILER "/home/fernandes/buildroot/buildroot-2021.02.5/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-g++")
#set(CMAKE_CC_COMPILER "/home/fernandes/buildroot/buildroot-2021.02.5/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-gcc")

#set(OpenCV_INCLUDE_DIRS "/usr/include/opencv2")

include_directories(include ${OpenCV_INCLUDE_DIRS})
#include_directories(include ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ )

#***********output variables***************
message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
message(STATUS "OpenCV_LIBRARIES = ${OpenCV_LIBRARIES}")
message(STATUS "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")

add_executable( testclassifier.elf ../testclassifier.cpp ${all_SRCS})
target_link_libraries( testclassifier.elf ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})

## Send binary file to tests's bin directory
#configure_file(testclassifier.elf ../tests/bin/testclassifier.elf COPYONLY)

#**********************************************************************************************************
#set(CMAKE_CXX_COMPILER "/usr/bin/g++")
#set(CMAKE_CC_COMPILER "/usr/bin/gcc")

#set(CMAKE_CXX_FLAGS "-isystem /home/fernandes/buildroot/buildroot-2021.02.5/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/include")

#set(OpenCV_INCLUDE_DIRS "/home/fernandes/buildroot/buildroot-2021.02.5/output/build/opencv3-3.4.13/buildroot-build/include")
#set(OpenCV_INCLUDE_DIRS "/usr/include/opencv4/opencv2/")
#set(OpenCV_INCLUDE_DIRS "/home/fernandes/buildroot/buildroot-2021.02.5/output/build/opencv3-3.4.13/buildroot-build" "/home/fernandes/buildroot/buildroot-2021.02.5/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/include")

#***********Libs***************
#set(OpenCV_LIBS "/home/fernandes/buildroot/buildroot-2021.02.5/output/build/opencv3-3.4.13/buildroot-build/lib/libopencv_videoio.so.3.4.13" "/home/fernandes/buildroot/buildroot-2021.02.5/output/build/opencv3-3.4.13/buildroot-build/lib/libopencv_videoio.so" "/home/fernandes/buildroot/buildroot-2021.02.5/output/build/opencv3-3.4.13/buildroot-build/lib/libopencv_videoio.so" "/home/fernandes/buildroot/buildroot-2021.02.5/output/build/opencv3-3.4.13/buildroot-build/lib/libopencv_videoio.so.3.4.13")
#set(OpenCV_LIBS libopencv_videoio.so.3.4.13 libopencv_videoio.so )

