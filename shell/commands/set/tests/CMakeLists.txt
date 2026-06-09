#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for set test files                                       #
#                                                                              #
#------------------------------------------------------------------------------#
set_property(GLOBAL APPEND PROPERTY
    ALL_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_set.cpp
    ${CMAKE_SOURCE_DIR}/src/shell/commands/set/set.c
    ${CMAKE_SOURCE_DIR}/src/shell/commands/set/set.h
)

add_executable(test_set
    ${CMAKE_CURRENT_SOURCE_DIR}/test_set.cpp
    ${CMAKE_SOURCE_DIR}/platforms/windows/test_main.cpp
)

target_link_libraries(test_set PRIVATE
    set_lib
    CppUTest::CppUTest
    CppUTestExt
)

add_test(NAME test_set COMMAND test_set)
