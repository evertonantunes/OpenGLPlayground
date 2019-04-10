cmake_minimum_required(VERSION 3.10)
project(resource)

include(rcc.cmake)

SET(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)

SET(HEADERS
	${INCLUDE_DIR}/resource.hpp
)

SET(SOURCES
        ${SOURCE_DIR}/resource.cpp
)

ADD_DEFINITIONS(-fsanitize=address)
ADD_DEFINITIONS(-fno-omit-frame-pointer)
ADD_DEFINITIONS(-O0)
ADD_DEFINITIONS(-Wall -Wextra -Werror)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} -lasan)
conan_target_link_libraries(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${INCLUDE_DIR}>" "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>")

add_subdirectory(rcc)

