cmake_minimum_required(VERSION 3.14)

# Версия на основе количества коммитов
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

set(PATCH_VERSION "${GIT_COMMIT_COUNT}" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 1.0.${PATCH_VERSION})

project(lab2 VERSION ${PROJECT_VERSION})

# Google Test вместо Boost
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Конфигурация версии
configure_file(version.h.in version.h)

# Основная программа (теперь main.cpp)
add_executable(lab2 main.cpp)
target_link_libraries(lab2 PRIVATE lab2_lib)

# Библиотека с версией
add_library(lab2_lib lib.cpp)
target_include_directories(lab2_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)

# Тесты
add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE gtest_main lab2_lib)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Копирование данных для тестов
configure_file(ip_filter.tsv ${CMAKE_CURRENT_BINARY_DIR}/ip_filter.tsv COPYONLY)

# Настройки компиляции
set_target_properties(lab2 lab2_lib tests PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(lab2 PRIVATE /W4)
    target_compile_options(lab2_lib PRIVATE /W4)
    target_compile_options(tests PRIVATE /W4)
else ()
    target_compile_options(lab2 PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(lab2_lib PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(tests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# Установка
install(TARGETS lab2 RUNTIME DESTINATION bin)

# Пакетирование
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

if(WIN32)
    set(CPACK_GENERATOR ZIP)
else()
    set(CPACK_GENERATOR DEB)
endif()

include(CPack)

# Тестирование
enable_testing()
add_test(NAME ip_filter_tests COMMAND tests)