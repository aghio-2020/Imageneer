cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

set(UTILS_LIB_NAME utils)
set(UTILS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ImageneerUtils/include)
set(UTILS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ImageneerUtils/src)
set(DEPENDENCIES_DIR ${CMAKE_SOURCE_DIR}/dependencies)
set(GLFW_DIR ${DEPENDENCIES_DIR}/GLFW)
set(IMGUI_DIR ${DEPENDENCIES_DIR}/imgui)

file(GLOB_RECURSE SOURCES
    "${UTILS_INCLUDE_DIR}/*.h"
    "${UTILS_SOURCE_DIR}/*.cpp"   
)

include_directories(${OpenCV_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)
add_library(${UTILS_LIB_NAME} ${SOURCES})

target_include_directories(${UTILS_LIB_NAME} PUBLIC ${IMGUI_DIR})
target_include_directories(${UTILS_LIB_NAME} PUBLIC ${GLFW_DIR}/include)
target_include_directories(${UTILS_LIB_NAME} PUBLIC ${UTILS_INCLUDE_DIR})
target_include_directories(${UTILS_LIB_NAME} PUBLIC ${UTILS_SOURCE_DIR})

#add a resource file to get all the common libraries easily accesible

target_link_libraries(${UTILS_LIB_NAME} PUBLIC ${OpenCV_LIBS} IMGUI)

if(WIN32)
    target_link_libraries(${UTILS_LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/glfw3.lib)
elseif(UNIX)
    target_link_libraries(${UTILS_LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/libglfw3.a)
endif()