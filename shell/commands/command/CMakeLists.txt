#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for command source files                                 #
#                                                                              #
#------------------------------------------------------------------------------#
add_library(command_lib STATIC
    ${CMAKE_CURRENT_LIST_DIR}/command.c
)

target_include_directories(command_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
