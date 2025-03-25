cmake_minimum_required(VERSION 3.20.0)
project(arena VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
    add_compile_options($<$<CONFIG:Debug>:/fsanitize=address>)
else()
    add_compile_options(-Wpedantic -Wall -Wextra -Werror)
    add_compile_options($<$<CONFIG:Release>:-O3> $<$<CONFIG:Release>:-ffast-math>)
    add_compile_options($<$<CONFIG:Debug>:-fsanitize=address> $<$<CONFIG:Debug>:-fsanitize=undefined>)
    add_link_options($<$<CONFIG:Debug>:-fsanitize=address> $<$<CONFIG:Debug>:-fsanitize=undefined>)
endif()

add_library(arena STATIC src/arena.c)
target_include_directories(arena PUBLIC include)
target_include_directories(arena PRIVATE src)

include(CTest)
enable_testing()
add_subdirectory(lib/unity)

add_executable(test_arena tests/test_arena.c)
target_link_libraries(test_arena unity arena)
target_include_directories(test_arena PRIVATE src)
add_test(TestArena test_arena)
