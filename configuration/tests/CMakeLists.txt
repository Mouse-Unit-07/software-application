#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for configuration test files                             #
#                                                                              #
#------------------------------------------------------------------------------#
set_property(GLOBAL APPEND PROPERTY
    ALL_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_configuration.cpp
    ${CMAKE_SOURCE_DIR}/src/configuration/configuration.c
    ${CMAKE_SOURCE_DIR}/src/configuration/configuration.h
)

add_executable(test_configuration
    ${CMAKE_CURRENT_SOURCE_DIR}/test_configuration.cpp
    ${CMAKE_SOURCE_DIR}/platforms/windows/test_main.cpp
)

target_link_libraries(test_configuration PRIVATE
    configuration_lib
    CppUTest::CppUTest
    CppUTestExt
)

add_test(NAME test_configuration COMMAND test_configuration)
