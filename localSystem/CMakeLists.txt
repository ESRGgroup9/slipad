cmake_minimum_required(VERSION 3.16.3)
project( slipad LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/TSL2581.c ${CMAKE_CURRENT_SOURCE_DIR}/src/DEV_Config.c PROPERTIES LANGUAGE CXX )

#opencv package
find_package( OpenCV REQUIRED )
#threads package
find_package( Threads REQUIRED )

# Compilers
set(CMAKE_CXX_COMPILER "$ENV{HOME}/buildroot/buildroot-2021.02.5/output/host/bin/arm-linux-g++")
set(CMAKE_CC_COMPILER "$ENV{HOME}/buildroot/buildroot-2021.02.5/output/host/bin/arm-linux-gcc")

# bin files to bin dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# OpenCv include DIRs
set(OpenCV_INCLUDE_DIRS "/usr/include/opencv2")

# BCM2835 flags
#set(CMAKE_CXX_FLAGS -D DEBUG -lbcm2835 -lrt)
add_compile_options(-D DEBUG)

add_compile_options(-lrt)
add_compile_options(-lbcm2835)


# including dirs
include_directories( include ${OpenCV_INCLUDE_DIRS} )
include_directories( include ${CMAKE_CURRENT_SOURCE_DIR}/inc/ )
include_directories( include ${CMAKE_CURRENT_SOURCE_DIR}/../inc )

# All Source files
file(GLOB all_SRCS
   "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
   "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp"
)

# Main
add_executable( localSys.elf localsys.cpp ${all_SRCS} ) 
target_link_libraries( localSys.elf ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_CXX_FLAGS} -lbcm2835 )

# DAEMON
add_executable( dSensors.elf dSensors.cpp ${all_SRCS} ) 
target_link_libraries( dSensors.elf ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_CXX_FLAGS} -lbcm2835 )

