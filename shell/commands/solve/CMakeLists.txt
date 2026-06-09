#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for solve source files                                   #
#                                                                              #
#------------------------------------------------------------------------------#
add_library(solve_lib STATIC
    ${CMAKE_CURRENT_LIST_DIR}/solve.c
)

target_include_directories(solve_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(solve_lib PUBLIC
    mouse_driver_lib
    maze_solver_lib
    configuration_lib
    command_lib
)
