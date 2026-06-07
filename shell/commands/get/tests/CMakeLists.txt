#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for get test files                                       #
#                                                                              #
#------------------------------------------------------------------------------#
set_property(GLOBAL APPEND PROPERTY
    ALL_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_get.cpp
    ${CMAKE_SOURCE_DIR}/src/shell/commands/get/get.c
    ${CMAKE_SOURCE_DIR}/src/shell/commands/get/get.h
)

add_executable(test_get
    ${CMAKE_CURRENT_SOURCE_DIR}/test_get.cpp
    ${CMAKE_SOURCE_DIR}/platforms/windows/test_main.cpp
)

target_link_libraries(test_get PRIVATE
    get_lib
    CppUTest::CppUTest
    CppUTestExt
)

add_test(NAME test_get COMMAND test_get)
