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
)

target_link_libraries(startup_lib PUBLIC
    mouse_driver_lib
    configuration_lib
)
