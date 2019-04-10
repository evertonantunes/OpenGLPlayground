

function(rcc_make_pakage_resource library_name EMBEDDED_FILES)
    set(RCC)
    if(UNIX)
        set(RCC rcc)
    elseif (WIN32)
        set(RCC rcc.exe)
    endif()

    SET(EMBEDDED_FILES ${${EMBEDDED_FILES}})

    foreach(EMBEDDED_FILE ${EMBEDDED_FILES})
        file(RELATIVE_PATH REGISTER_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR} ${EMBEDDED_FILE})
        get_filename_component(PATH ${EMBEDDED_FILE} DIRECTORY)
        get_filename_component(ORG_FILE_NAME ${EMBEDDED_FILE} NAME)

        string(REPLACE "." "_" FILE_NAME_1 ${ORG_FILE_NAME})
        string(REPLACE "-" "_" FILE_NAME_2 ${FILE_NAME_1})
        string(REPLACE "/" "_" FILE_NAME_3 ${FILE_NAME_2})
        string(REPLACE "\\" "_" FILE_NAME ${FILE_NAME_3})

        set(PATH_FILE_NAME "${PATH}/${FILE_NAME}.cpp")

        add_custom_command(
            OUTPUT ${PATH_FILE_NAME}
            COMMAND ${RCC} ${EMBEDDED_FILE} ${PATH_FILE_NAME} ${REGISTER_FILE_NAME}
            COMMENT ${RCC} ${EMBEDDED_FILE} ${PATH_FILE_NAME} ${REGISTER_FILE_NAME}            
            MAIN_DEPENDENCY ${EMBEDDED_FILE}
            )
        list(APPEND COMPONENT_SOURCES ${PATH_FILE_NAME})
    endforeach()

    set(RESOURCES_TO_COMPILE ${COMPONENT_SOURCES} PARENT_SCOPE)

endfunction(rcc_make_pakage_resource)
