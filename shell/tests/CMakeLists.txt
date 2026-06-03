#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for shell test files                                     #
#                                                                              #
#------------------------------------------------------------------------------#
set_property(GLOBAL APPEND PROPERTY
    ALL_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_shell.cpp
    ${CMAKE_SOURCE_DIR}/src/shell/shell.c
    ${CMAKE_SOURCE_DIR}/src/shell/shell.h
)

add_executable(test_shell
    ${CMAKE_CURRENT_SOURCE_DIR}/test_shell.cpp
    ${CMAKE_SOURCE_DIR}/platforms/windows/test_main.cpp
)

target_link_libraries(test_shell PRIVATE
    shell_lib
    CppUTest::CppUTest
    CppUTestExt
)

add_test(NAME test_shell COMMAND test_shell)
