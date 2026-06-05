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

add_subdirectory(shell/commands)
add_subdirectory(shell/core)
add_subdirectory(startup)

# \/=== Add all test directories
if(TARGET_WINDOWS AND SUPPORTS_WINDOWS)
    if(ENABLE_APPLICATION_TESTS)
        add_subdirectory(shell/commands/tests)
        add_subdirectory(shell/core/tests)
        add_subdirectory(startup/tests)
    endif()
endif()

add_library(application_lib INTERFACE)

target_link_libraries(application_lib INTERFACE
    commands_lib
    core_lib
    startup_lib
)
