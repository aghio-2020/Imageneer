cmake_minimum_required (VERSION 3.8)

project(ComputerVision)

set(CMAKE_CXX_STANDARD 20)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ComputerVision/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/ComputerVision/src)

file(GLOB CV_SOURCES
    "${INCLUDE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp")

# Open CV
################################

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

################################

add_library(${PROJECT_NAME} STATIC ${CV_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS})