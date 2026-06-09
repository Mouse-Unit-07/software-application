#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# Top level CMakeLists.txt file (for this software repo)                       #
#                                                                              #
#------------------------------------------------------------------------------#
cmake_minimum_required(VERSION 4.0.2)
project(application_project LANGUAGES C)

include(FetchContent)

FetchContent_Declare(
    maze_solver
    GIT_REPOSITORY git@github.com:Mouse-Unit-07/software-maze-solver.git
    GIT_TAG main
)
FetchContent_MakeAvailable(maze_solver)

add_subdirectory(configuration)
add_subdirectory(shell/commands/command)
add_subdirectory(shell/commands/get)
add_subdirectory(shell/commands/root)
add_subdirectory(shell/commands/set)
add_subdirectory(shell/commands/solve)
add_subdirectory(shell/commands/test_device)
add_subdirectory(shell/core)
add_subdirectory(startup)

# \/=== Add all test directories
if(TARGET_WINDOWS AND SUPPORTS_WINDOWS)
    if(ENABLE_APPLICATION_TESTS)
        add_subdirectory(configuration/tests)
        add_subdirectory(shell/commands/command/tests)
        add_subdirectory(shell/commands/get/tests)
        add_subdirectory(shell/commands/root/tests)
        add_subdirectory(shell/commands/set/tests)
        add_subdirectory(shell/commands/solve/tests)
        add_subdirectory(shell/commands/test_device/tests)
        add_subdirectory(shell/core/tests)
        add_subdirectory(startup/tests)
    endif()
endif()

add_library(application_lib INTERFACE)

target_link_libraries(application_lib INTERFACE
    configuration_lib
    root_lib
    core_lib
    startup_lib
)
