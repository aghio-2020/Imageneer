cmake_minimum_required (VERSION 3.8)

project(ImageneerClient)

set(CMAKE_CXX_STANDARD 20)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ImageneerClient/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/ImageneerClient/src)
set(DEPENDENCIES_DIR ${CMAKE_SOURCE_DIR}/dependencies)
set(GLFW_DIR ${DEPENDENCIES_DIR}/GLFW)
set(GLAD_DIR ${DEPENDENCIES_DIR}/glad)
set(IMGUI_DIR ${DEPENDENCIES_DIR}/imgui)
set(NFD_DIR ${DEPENDENCIES_DIR}/nativefiledialog)
set(STB_DIR ${DEPENDENCIES_DIR}/stb)
set(OpenGL_GL_PREFERENCE GLVND)

set(IMAGERECOGNITION_DIR ${CMAKE_SOURCE_DIR}/ImageRecognition)

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
target_include_directories(IMGUI PRIVATE ${GLFW_DIR}/include)
target_include_directories(IMGUI PRIVATE ${GLAD_DIR})

add_library(glad STATIC "${DEPENDENCIES_DIR}/glad/glad.h" "${DEPENDENCIES_DIR}/glad/glad.c")
target_include_directories(glad PRIVATE ${DEPENDENCIES_DIR})
target_include_directories(glad PRIVATE ${GLFW_DIR})

# Add source to this project's executable.
add_executable (${PROJECT_NAME} ${SOURCES} "app.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC ${DEPENDENCIES_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${NFD_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${STB_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${IMAGERECOGNITION_DIR}/include)

if(WIN32)
	target_link_libraries(IMGUI kernel32 user32 gdi32 opengl32 ${DEPENDENCIES_DIR}/GLFW/lib/glfw3.lib)
	target_link_libraries(glad ${DEPENDENCIES_DIR}/GLFW/lib/glfw3.lib)
	target_link_libraries(${PROJECT_NAME} ImageRecognition IMGUI glad kernel32 user32 gdi32 opengl32 ${NFD_DIR}/lib/nfd.lib ${DEPENDENCIES_DIR}/GLFW/lib/glfw3.lib)
elseif(UNIX)
	target_link_libraries(IMGUI pthread ${OPENGL_gl_LIBRARY} dl ${DEPENDENCIES_DIR}/GLFW/lib/libglfw3.a)
	target_link_libraries(glad pthread ${OPENGL_gl_LIBRARY} dl ${DEPENDENCIES_DIR}/GLFW/lib/libglfw3.a)
	target_link_libraries(${PROJECT_NAME} ImageRecognition IMGUI glad ${NFD_DIR}/lib/libnfd.a ${OPENGL_gl_LIBRARY} dl ${DEPENDENCIES_DIR}/GLFW/lib/libglfw3.a)
endif()

