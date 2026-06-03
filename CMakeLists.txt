#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# Top level CMakeLists.txt file (for this software repo)                       #
#                                                                              #
#------------------------------------------------------------------------------#
cmake_minimum_required(VERSION 4.0.2)
project(repeat_hello_world_project LANGUAGES C) # <== Name your software project

include(FetchContent)

# \/=== Add dependencies to other GitHub repos
# Example dependency on Hello World project:
FetchContent_Declare(
    print_hello_world
    GIT_REPOSITORY git@github.com:Mouse-Unit-07/experiment-software-hello-world.git
    GIT_TAG main
)
FetchContent_MakeAvailable(print_hello_world)

# \/=== Add all library directories (each w/ a CMakeLists.txt)
add_subdirectory(repeat_hello_world)

# \/=== Add all test directories
if(TARGET_WINDOWS AND SUPPORTS_WINDOWS)
    if(ENABLE_REPEAT_HELLO_WORLD_TESTS)
        add_subdirectory(repeat_hello_world/tests)
    endif()
endif()

# \/=== Create an aggregate interface library if your project has multiple libraries 
add_library(repeat_hello_world_aggregate_lib INTERFACE)

# \/=== Link to all libraries defined by your software project
target_link_libraries(repeat_hello_world_aggregate_lib INTERFACE
    repeat_hello_world_lib
)
