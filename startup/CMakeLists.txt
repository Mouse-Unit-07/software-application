#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for startup source files                                 #
#                                                                              #
#------------------------------------------------------------------------------#
add_library(startup_lib STATIC
    ${CMAKE_CURRENT_LIST_DIR}/startup.c
)

target_include_directories(startup_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    $<TARGET_PROPERTY:mouse_driver_lib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:maze_solver_lib,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:configuration_lib,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(startup_lib PRIVATE
    mouse_driver_lib
    maze_solver_lib
    configuration_lib
)
