cmake_minimum_required(VERSION 3.10)
SET(THIS_PROJECT_NAME "intro")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

SET(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)

SET(HEADERS

)

SET(SOURCES
        ${SOURCE_DIR}/main.cpp
)

ADD_DEFINITIONS(-fsanitize=address)
ADD_DEFINITIONS(-fno-omit-frame-pointer)
ADD_DEFINITIONS(-O0)
ADD_DEFINITIONS(-Wall -Wextra -Werror)

add_executable(${THIS_PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${THIS_PROJECT_NAME} -lasan utility ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
conan_target_link_libraries(${THIS_PROJECT_NAME})

