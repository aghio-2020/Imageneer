cmake_minimum_required (VERSION 3.8)

project(ImageRecognition)

set(CMAKE_CXX_STANDARD 20)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ImageRecognition/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/ImageRecognition/src)

# Open CV
################################

# Change this directory to your installed OpenCv
set(OPENCV_DIR D:/repos/opencv-4.6.0-download/opencv)
# Change to the library with the correct version
set(OPENCV_LIB_FILE_WINDOWS ${OPENCV_DIR}/build/x64/vc15/lib/opencv_world460.lib)
# TODO: set(OPENCV_LIB_FILE_LINUX) 

set(OPENCV_INCLUDE_DIR ${OPENCV_DIR}/build/include)

################################

add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME}
PRIVATE
    ${INCLUDE_DIR}/ImageRecognition.h 
    ${SRC_DIR}/ImageRecognition.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENCV_INCLUDE_DIR})

#TODO: Add support for Linux
target_link_libraries(${PROJECT_NAME} ${OPENCV_LIB_FILE_WINDOWS})