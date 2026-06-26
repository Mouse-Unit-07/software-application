#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for core source files                                    #
#                                                                              #
#------------------------------------------------------------------------------#
add_library(core_lib STATIC
    ${CMAKE_CURRENT_LIST_DIR}/core.c
)

target_include_directories(core_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    $<TARGET_PROPERTY:mouse_driver_lib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:command_lib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:root_lib,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(core_lib PRIVATE
    mouse_driver_lib
    command_lib
    root_lib
)
