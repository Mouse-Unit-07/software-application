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
    $<TARGET_PROPERTY:mouse_driver_lib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:get_lib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:set_lib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:solve_lib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:test_device_lib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:command_lib,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(root_lib PRIVATE
    mouse_driver_lib
    get_lib
    set_lib
    solve_lib
    test_device_lib
    command_lib
)
