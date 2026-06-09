#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for get source files                                     #
#                                                                              #
#------------------------------------------------------------------------------#
add_library(get_lib STATIC
    ${CMAKE_CURRENT_LIST_DIR}/get.c
)

target_include_directories(get_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(get_lib PUBLIC
    maze_solver_lib
    configuration_lib
    command_lib
)
