#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for core test files                                      #
#                                                                              #
#------------------------------------------------------------------------------#
set_property(GLOBAL APPEND PROPERTY
    ALL_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_core.cpp
    ${CMAKE_SOURCE_DIR}/src/shell/core/core.c
    ${CMAKE_SOURCE_DIR}/src/shell/core/core.h
)

add_executable(test_core
    ${CMAKE_CURRENT_SOURCE_DIR}/test_core.cpp
    ${CMAKE_SOURCE_DIR}/platforms/windows/test_main.cpp
)

target_link_libraries(test_core PRIVATE
    core_lib
    CppUTest::CppUTest
    CppUTestExt
)

add_test(NAME test_core COMMAND test_core)
