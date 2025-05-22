cmake_minimum_required(VERSION 3.12)
set(OpenGL_GL_PREFERENCE LEGACY)

project(mc)
add_compile_definitions(GLEW_STATIC)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
    ${CMAKE_SOURCE_DIR}/src/*.cpp 
    ${CMAKE_SOURCE_DIR}/src/*.hpp
    ${CMAKE_SOURCE_DIR}/src/*.h
)

if (WIN32)  # Windows

include_directories(
    vendor/GLFW/include
    vendor/GLEW/include
    vendor/stb_image
    vendor/GLM
)

link_directories(
    vendor/GLFW/lib-mingw-w64
    vendor/GLEW/lib
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} glfw3 glew32s opengl32)

else()  # Linux

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} GL)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} GLEW)

endif()