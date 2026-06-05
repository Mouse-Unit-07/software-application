#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for commands source files                                #
#                                                                              #
#------------------------------------------------------------------------------#
add_library(commands_lib STATIC
    ${CMAKE_CURRENT_LIST_DIR}/commands.c
)

target_include_directories(commands_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(commands_lib PRIVATE
    mouse_driver_lib
    maze_solver_lib
)
