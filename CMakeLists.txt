cmake_minimum_required(VERSION 3.10)

find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND git rev-list --count HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_COUNT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
else()
    set(GIT_COMMIT_COUNT "0")
endif()

set(PROJECT_VERSION 1.0.${GIT_COMMIT_COUNT})
project(lab2 VERSION ${PROJECT_VERSION})

if(WIN32)
    if(MSVC)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    endif()
endif()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "Use shared (DLL) run-time lib" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(lab2 main.cpp)

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE GTest::gtest_main)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

configure_file(ip_filter.tsv ${CMAKE_CURRENT_BINARY_DIR}/ip_filter.tsv COPYONLY)

set_target_properties(lab2 tests PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(lab2 PRIVATE /W4)
    target_compile_options(tests PRIVATE /W4 /MD)
else ()
    target_compile_options(lab2 PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(tests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS lab2 RUNTIME DESTINATION .)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

if(WIN32)
    set(CPACK_GENERATOR ZIP)
else()
    set(CPACK_GENERATOR DEB)
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "lab2")
endif()

include(CPack)

enable_testing()
add_test(NAME ip_filter_tests COMMAND tests)