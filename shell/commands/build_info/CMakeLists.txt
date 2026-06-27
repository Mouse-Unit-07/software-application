#--------------------------------- FILE INFO ----------------------------------#
# Filename           : CMakeLists.txt                                          #
#                                                                              #
# CMakeLists.txt file for build_info source files                              #
#                                                                              #
#------------------------------------------------------------------------------#
add_library(build_info_lib STATIC
    ${CMAKE_CURRENT_LIST_DIR}/build_info.c
    ${CMAKE_CURRENT_BINARY_DIR}/build_info_template.c
)

# ------------------------------------------------------------------------------
# exclusive build_info parsing logic

find_package(Git REQUIRED)

# time, build configuration, hardware target
string(TIMESTAMP BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP BUILD_TIME "%H:%M:%S")
set(BUILD_CONFIGURATION "${CMAKE_BUILD_TYPE}")
set(BUILD_MOUSE_TARGET "${MOUSE_TARGET}")

# find all git repos utilized
set(REPOSITORIES)

if(EXISTS "${CMAKE_SOURCE_DIR}/src/.git")
    list(APPEND REPOSITORIES "${CMAKE_SOURCE_DIR}/src")
endif()

file(GLOB_RECURSE ALL_PATHS
    LIST_DIRECTORIES TRUE
    "${CMAKE_SOURCE_DIR}/*"
)

foreach(PATH ${ALL_PATHS})
    if(NOT IS_DIRECTORY "${PATH}")
        continue()
    endif()

    if(NOT EXISTS "${PATH}/.git")
        continue()
    endif()

    execute_process(
        COMMAND ${GIT_EXECUTABLE} -C "${PATH}" rev-parse --show-toplevel
        OUTPUT_VARIABLE REPO_ROOT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    if(REPO_ROOT)
        list(APPEND REPOSITORIES "${REPO_ROOT}")
    endif()
endforeach()

list(SORT REPOSITORIES)
list(REMOVE_DUPLICATES REPOSITORIES)

# parse for git URL's and commit hashes
set(BUILD_REPOSITORIES)
set(SEEN_URLS)

foreach(REPO ${REPOSITORIES})
    execute_process(
        COMMAND ${GIT_EXECUTABLE} -C "${REPO}" remote get-url origin
        RESULT_VARIABLE RESULT
        OUTPUT_VARIABLE URL
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    if(NOT RESULT EQUAL 0)
        continue()
    endif()

    get_filename_component(NAME "${URL}" NAME_WE)

    list(FIND SEEN_URLS "${URL}" URL_INDEX)

    if(NOT URL_INDEX EQUAL -1)
        continue()
    endif()

    if(URL STREQUAL "")
        continue()
    endif()

    list(APPEND SEEN_URLS "${URL}")

    execute_process(
        COMMAND ${GIT_EXECUTABLE} -C "${REPO}" rev-parse --short HEAD
        RESULT_VARIABLE RESULT
        OUTPUT_VARIABLE HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    if(NOT RESULT EQUAL 0)
        continue()
    endif()

    string(APPEND BUILD_REPOSITORIES
"    {
        \"${NAME}\",
        \"${URL}\",
        \"${HASH}\"
    },
")
endforeach()

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/build_info_template.c.in
    ${CMAKE_CURRENT_BINARY_DIR}/build_info_template.c
    @ONLY
)
# ------------------------------------------------------------------------------

target_include_directories(build_info_lib PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_LIST_DIR}
    $<TARGET_PROPERTY:command_lib,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(build_info_lib PRIVATE
    command_lib
)
