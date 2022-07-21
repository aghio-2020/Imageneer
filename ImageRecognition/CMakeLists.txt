# CMakeList.txt : CMake project for ImageRecognition
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/ImageRecognition/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/ImageRecognition/src")
set(DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/dependencies")
set(GLFW_DIR "${CMAKE_SOURCE_DIR}/dependencies/GLFW/include")
set(GLAD_DIR "${DEPENDENCIES_DIR}/glad")
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/dependencies/imgui)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES
 	 "${INCLUDE_DIR}/*.h"
     "${SOURCE_DIR}/*.h"
     "${SOURCE_DIR}/*.cpp")

add_library(IMGUI STATIC)
target_sources(IMGUI
PRIVATE
	${IMGUI_DIR}/imgui_demo.cpp
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_tables.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
	${IMGUI_DIR}/imgui.cpp
PRIVATE
	${IMGUI_DIR}/imgui_impl_opengl3.cpp
	${IMGUI_DIR}/imgui_impl_glfw.cpp
)
target_include_directories(IMGUI PUBLIC ${IMGUI_DIR})
target_include_directories(IMGUI PRIVATE ${GLFW_DIR})
target_include_directories(IMGUI PRIVATE ${GLAD_DIR})

add_library(glad STATIC "${DEPENDENCIES_DIR}/glad/glad.h" "${DEPENDENCIES_DIR}/glad/glad.c")
target_include_directories(glad PRIVATE ${DEPENDENCIES_DIR})
target_include_directories(glad PRIVATE ${GLFW_DIR})
target_include_directories(glad PRIVATE ${IMGUI_DIR})
target_include_directories(glad PRIVATE ${SOURCE_DIR})
target_include_directories(glad PRIVATE ${INCLUDE_DIR})

# Add source to this project's executable.
add_executable (${PROJECT_NAME} ${SOURCES} "app.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC ${DEPENDENCIES_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

if(WIN32)
	target_link_libraries(IMGUI kernel32 user32 gdi32 opengl32 "${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/glfw3.lib")
	target_link_libraries(glad "${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/glfw3.lib")
	target_link_libraries(${PROJECT_NAME} IMGUI glad kernel32 user32 gdi32 opengl32 "${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/glfw3.lib")
elseif(UNIX)
	target_link_libraries(IMGUI pthread ${OPENGL_gl_LIBRARY} dl ${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/libglfw3.a)
	target_link_libraries(glad pthread ${OPENGL_gl_LIBRARY} dl ${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/libglfw3.a)
	target_link_libraries(${PROJECT_NAME} IMGUI glad ${OPENGL_gl_LIBRARY} dl ${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/libglfw3.a)
endif()
