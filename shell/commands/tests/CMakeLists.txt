#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for commands test files                                  #
#                                                                              #
#------------------------------------------------------------------------------#
set_property(GLOBAL APPEND PROPERTY
    ALL_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_commands.cpp
    ${CMAKE_SOURCE_DIR}/src/shell/commands/commands.c
    ${CMAKE_SOURCE_DIR}/src/shell/commands/commands.h
)

add_executable(test_commands
    ${CMAKE_CURRENT_SOURCE_DIR}/test_commands.cpp
    ${CMAKE_SOURCE_DIR}/platforms/windows/test_main.cpp
)

target_link_libraries(test_commands PRIVATE
    commands_lib
    CppUTest::CppUTest
    CppUTestExt
)

add_test(NAME test_commands COMMAND test_commands)
