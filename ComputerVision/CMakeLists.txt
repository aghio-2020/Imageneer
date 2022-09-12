cmake_minimum_required (VERSION 3.8)

project(ComputerVision)

set(CMAKE_CXX_STANDARD 20)

set(COMPUTERVISION_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ComputerVision/include)
set(COMPUTERVISION_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ComputerVision/src)
set(UTILS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ImageneerUtils/include)

file(GLOB CV_SOURCES
    "${COMPUTERVISION_INCLUDE_DIR}/*.h"
    "${COMPUTERVISION_SOURCE_DIR}/*.cpp")

# Open CV
################################

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

################################

add_library(${PROJECT_NAME} STATIC ${CV_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${COMPUTERVISION_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${COMPUTERVISION_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${UTILS_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC utils ${OpenCV_LIBS})