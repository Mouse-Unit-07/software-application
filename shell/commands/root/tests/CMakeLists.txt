#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for root test files                                      #
#                                                                              #
#------------------------------------------------------------------------------#
set_property(GLOBAL APPEND PROPERTY
    ALL_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_root.cpp
    ${CMAKE_SOURCE_DIR}/src/shell/commands/root/root.c
    ${CMAKE_SOURCE_DIR}/src/shell/commands/root/root.h
)

add_executable(test_root
    ${CMAKE_CURRENT_SOURCE_DIR}/test_root.cpp
    ${CMAKE_SOURCE_DIR}/platforms/windows/test_main.cpp
)

target_link_libraries(test_root PRIVATE
    root_lib
    CppUTest::CppUTest
    CppUTestExt
)

add_test(NAME test_root COMMAND test_root)
