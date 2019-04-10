cmake_minimum_required(VERSION 3.10)
project(ripple_mesh)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

SET(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
SET(RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resource)

SET(HEADERS

)

SET(SOURCES
        ${SOURCE_DIR}/main.cpp
)

SET(RESOURCES
        ${RESOURCE_DIR}/shader.frag
        ${RESOURCE_DIR}/shader.vert
)

ADD_DEFINITIONS(-fsanitize=address)
ADD_DEFINITIONS(-fno-omit-frame-pointer)
ADD_DEFINITIONS(-O0)
ADD_DEFINITIONS(-Wall -Wextra -Werror)

rcc_make_pakage_resource(resource_ripple_mesh RESOURCES)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES} ${RESOURCES_TO_COMPILE})
target_link_libraries(${PROJECT_NAME} -lasan resource utility ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
conan_target_link_libraries(${PROJECT_NAME})
