#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for root source files                                    #
#                                                                              #
#------------------------------------------------------------------------------#
add_library(root_lib STATIC
    ${CMAKE_CURRENT_LIST_DIR}/root.c
)

target_include_directories(root_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(root_lib PUBLIC
    mouse_driver_lib
    get_lib
    set_lib
    solve_lib
    test_device_lib
    command_lib
)
