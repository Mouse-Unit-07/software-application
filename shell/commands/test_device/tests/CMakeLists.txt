#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for test_device test files                               #
#                                                                              #
#------------------------------------------------------------------------------#
set_property(GLOBAL APPEND PROPERTY
    ALL_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_test_device.cpp
    ${CMAKE_SOURCE_DIR}/src/shell/commands/test_device/test_device.c
    ${CMAKE_SOURCE_DIR}/src/shell/commands/test_device/test_device.h
)

add_executable(test_test_device
    ${CMAKE_CURRENT_SOURCE_DIR}/test_test_device.cpp
    ${CMAKE_SOURCE_DIR}/platforms/windows/test_main.cpp
)

target_link_libraries(test_test_device PRIVATE
    test_device_lib
    CppUTest::CppUTest
    CppUTestExt
)

add_test(NAME test_test_device COMMAND test_test_device)
