#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for repeat_hello_world test files                        #
#                                                                              #
#------------------------------------------------------------------------------#
# \/=== Add all test and associated tested source and header files
set_property(GLOBAL APPEND PROPERTY
    ALL_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_repeat_hello_world.cpp
    ${CMAKE_SOURCE_DIR}/src/repeat_hello_world/repeat_hello_world.c
    ${CMAKE_SOURCE_DIR}/src/repeat_hello_world/repeat_hello_world.h
)

add_executable(test_repeat_hello_world # <=== Define your test target
    ${CMAKE_CURRENT_SOURCE_DIR}/test_repeat_hello_world.cpp # <=== Add all test source files
    ${CMAKE_SOURCE_DIR}/platforms/windows/test_main.cpp
)

target_link_libraries(test_repeat_hello_world PRIVATE # <=== Rename to your test target
    repeat_hello_world_lib # <== Link to the library being tested in this test executable
    CppUTest::CppUTest
    CppUTestExt
)

add_test(NAME test_repeat_hello_world COMMAND test_repeat_hello_world) # <=== Rename to your test target
