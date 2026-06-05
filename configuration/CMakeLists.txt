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
)

target_link_libraries(configuration_lib PUBLIC
    mouse_driver_lib
    maze_solver_lib
)
