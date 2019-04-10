project(rcc)
cmake_minimum_required(VERSION 2.6)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)

set(SOURCES
    ${SOURCE_DIR}/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} -lasan)


