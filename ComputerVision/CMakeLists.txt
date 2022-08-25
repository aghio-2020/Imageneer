cmake_minimum_required (VERSION 3.8)

project(ComputerVision)

set(CMAKE_CXX_STANDARD 20)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ComputerVision/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/ComputerVision/src)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# Open CV
################################

# Change this directory to your installed OpenCv
set(OPENCV_DIR D:/repos/opencv-4.6.0-download/opencv)
# Change to the library with the correct version
set(OPENCV_LIB_FILE_WINDOWS ${OPENCV_DIR}/build/x64/vc15/lib/opencv_world460.lib)
# TODO: set(OPENCV_LIB_FILE_LINUX) 

set(OPENCV_INCLUDE_DIR ${OPENCV_DIR}/build/include)

################################

add_library(${PROJECT_NAME} STATIC "${SRC_DIR}/ComputerVisionFunc.cpp" "${INCLUDE_DIR}/ComputerVisionFunc.h")
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENCV_INCLUDE_DIR})

#TODO: Add support for Linux
target_link_libraries(${PROJECT_NAME} ${OPENCV_LIB_FILE_WINDOWS})