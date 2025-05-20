cmake_minimum_required(VERSION 3.12)
set(OpenGL_GL_PREFERENCE LEGACY)

project(mc)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
    ${CMAKE_SOURCE_DIR}/src/*.cpp 
    ${CMAKE_SOURCE_DIR}/src/*.hpp
    ${CMAKE_SOURCE_DIR}/src/*.h
)

if (WIN32)  # Windows

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
file(GLOB IMGUI_SRC CONFIGURE_DEPENDS
    ${IMGUI_DIR}/imgui*.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_demo.cpp
)
add_library(imgui
    ${IMGUI_SRC}
)
target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

include_directories(
    vendor/GLFW/include
    vendor/GLEW/include
    vendor/stb_image
    vendor/GLM
    vendor/imgui
    vendor/imgui/backends
)

link_directories(
    vendor/GLFW/lib-mingw-w64
    vendor/GLEW/lib
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} glfw3 glew32s opengl32 imgui)
add_compile_definitions(GLEW_STATIC)

else()  # Linux

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} GL)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} GLEW)

endif()
