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
)

target_link_libraries(core_lib PUBLIC
    mouse_driver_lib
    command_lib
    root_lib
)
