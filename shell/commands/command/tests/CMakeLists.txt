#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for command test files                                   #
#                                                                              #
#------------------------------------------------------------------------------#
set_property(GLOBAL APPEND PROPERTY
    ALL_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_command.cpp
    ${CMAKE_SOURCE_DIR}/src/shell/commands/command/command.c
    ${CMAKE_SOURCE_DIR}/src/shell/commands/command/command.h
)

add_executable(test_command
    ${CMAKE_CURRENT_SOURCE_DIR}/test_command.cpp
    ${CMAKE_SOURCE_DIR}/platforms/windows/test_main.cpp
)

target_link_libraries(test_command PRIVATE
    command_lib
    CppUTest::CppUTest
    CppUTestExt
)

add_test(NAME test_command COMMAND test_command)
