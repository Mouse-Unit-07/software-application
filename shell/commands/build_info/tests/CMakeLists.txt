#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for build_info test files                                #
#                                                                              #
#------------------------------------------------------------------------------#
set_property(GLOBAL APPEND PROPERTY
    ALL_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_build_info.cpp
    ${CMAKE_SOURCE_DIR}/src/shell/commands/build_info/build_info.c
    ${CMAKE_SOURCE_DIR}/src/shell/commands/build_info/build_info.h
)

add_executable(test_build_info
    ${CMAKE_CURRENT_SOURCE_DIR}/test_build_info.cpp
    ${CMAKE_SOURCE_DIR}/platforms/windows/test_main.cpp
)

target_link_libraries(test_build_info PRIVATE
    build_info_lib
    CppUTest::CppUTest
    CppUTestExt
)

add_test(NAME test_build_info COMMAND test_build_info)
