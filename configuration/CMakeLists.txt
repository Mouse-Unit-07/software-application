#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for configuration source files                           #
#                                                                              #
#------------------------------------------------------------------------------#
add_library(configuration_lib STATIC
    ${CMAKE_CURRENT_LIST_DIR}/configuration.c
)

target_include_directories(configuration_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    $<TARGET_PROPERTY:mouse_driver_lib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:maze_solver_lib,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(configuration_lib PRIVATE
    mouse_driver_lib
    maze_solver_lib
)
