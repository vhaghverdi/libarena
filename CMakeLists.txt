cmake_minimum_required(VERSION 3.25.0)
project(arena VERSION 1.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(arena STATIC src/arena.c)
target_include_directories(arena PUBLIC include)
target_include_directories(arena PRIVATE src)

if(MSVC)
	target_compile_options(arena PRIVATE
		/W4 /WX /std:clatest /guard:cf /analyze /sdl /permissive-
		$<$<CONFIG:Debug>:/fsanitize=address>
		$<$<CONFIG:Debug>:/Od>
		$<$<CONFIG:Release>:/O2>
	)
else()
	target_compile_options(arena PRIVATE
		-pedantic -Wall -Wextra -Werror -Wconversion -Wformat-security
		-ftrivial-auto-var-init=zero -fstack-clash-protection -fpie -Wl,-pie
		-fpic -shared -Wl,-z,noexecstack -fstack-protector-strong
		$<$<CONFIG:Debug>:-D_FORTIFY_SOURCE=3>
		$<$<CONFIG:Debug>:-g3>
		$<$<CONFIG:Debug>:-fsanitize=address>
		$<$<CONFIG:Debug>:-fsanitize=undefined>
		$<$<CONFIG:Release>:-O2>
	)
	target_link_options(arena PRIVATE
		$<$<CONFIG:Debug>:-fsanitize=address>
		$<$<CONFIG:Debug>:-fsanitize=undefined>
	)
endif()
		
include(CTest)
enable_testing()
add_subdirectory(lib/unity)

add_executable(test_arena tests/test_arena.c)
target_link_libraries(test_arena unity arena)
target_include_directories(test_arena PRIVATE src)
add_test(TestArena test_arena)
