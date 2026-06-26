#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for test_device source files                             #
#                                                                              #
#------------------------------------------------------------------------------#
add_library(test_device_lib STATIC
    ${CMAKE_CURRENT_LIST_DIR}/test_device.c
)

target_include_directories(test_device_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    $<TARGET_PROPERTY:mouse_driver_lib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:maze_solver_lib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:configuration_lib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:command_lib,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(test_device_lib PRIVATE
    mouse_driver_lib
    maze_solver_lib
    configuration_lib
    command_lib
)
