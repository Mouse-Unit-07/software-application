#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for startup test files                                   #
#                                                                              #
#------------------------------------------------------------------------------#
set_property(GLOBAL APPEND PROPERTY
    ALL_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_startup.cpp
    ${CMAKE_SOURCE_DIR}/src/startup/startup.c
    ${CMAKE_SOURCE_DIR}/src/startup/startup.h
)

add_executable(test_startup
    ${CMAKE_CURRENT_SOURCE_DIR}/test_startup.cpp
    ${CMAKE_SOURCE_DIR}/platforms/windows/test_main.cpp
)

target_link_libraries(test_startup PRIVATE
    startup_lib
    CppUTest::CppUTest
    CppUTestExt
)

add_test(NAME test_startup COMMAND test_startup)
