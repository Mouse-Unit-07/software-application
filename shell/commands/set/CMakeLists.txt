#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for set source files                                     #
#                                                                              #
#------------------------------------------------------------------------------#
add_library(set_lib STATIC
    ${CMAKE_CURRENT_LIST_DIR}/set.c
)

target_include_directories(set_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    $<TARGET_PROPERTY:maze_solver_lib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:configuration_lib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:command_lib,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(set_lib PRIVATE
    maze_solver_lib
    configuration_lib
    command_lib
)
