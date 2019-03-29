cmake_minimum_required(VERSION 3.10)
project(utility)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

SET(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)

SET(HEADERS
	${INCLUDE_DIR}/shader.hpp
)

SET(SOURCES
        ${SOURCE_DIR}/shader.cpp
)

ADD_DEFINITIONS(-fsanitize=address)
ADD_DEFINITIONS(-fno-omit-frame-pointer)
ADD_DEFINITIONS(-O0)
ADD_DEFINITIONS(-Wall -Wextra -Werror)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} -lasan ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
conan_target_link_libraries(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${INCLUDE_DIR}>" "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>")
