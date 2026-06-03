#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for shell source files                                   #
#                                                                              #
#------------------------------------------------------------------------------#
add_library(shell_lib STATIC
    ${CMAKE_CURRENT_LIST_DIR}/shell.c
)

target_include_directories(shell_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(shell_lib PUBLIC
    mouse_driver_lib
    maze_solver_lib
)
