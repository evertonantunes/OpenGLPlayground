project(OpenGLPlayground)
cmake_minimum_required(VERSION 3.10)

include(conanbuildinfo.cmake)
conan_basic_setup()

add_definitions(-O0)

SET(CMAKE_CXX_STANDARD 17)

add_subdirectory(rcc)
add_subdirectory(utility)
add_subdirectory(projects)

