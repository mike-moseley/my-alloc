cmake_minimum_required(VERSION 3.10)
project(c-project)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_compile_definitions(_DEFAULT_SOURCE)

file(GLOB_RECURSE sources
		"${CMAKE_SOURCE_DIR}/src/*.c"
)
add_executable(app ${sources})
target_include_directories(app PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_options(app PRIVATE -Wall -Wextra -g -pedantic)

# Unity testing
enable_testing()

set(UNITY_SRC "${CMAKE_SOURCE_DIR}/test/unity/unity.c")

file(GLOB_RECURSE test_sources
	"${CMAKE_SOURCE_DIR}/src/*.c"
)
list(FILTER test_sources EXCLUDE REGEX ".*/main\\.c$")

function(add_unity_test target source)
	add_executable(${target} ${source} ${UNITY_SRC} ${test_sources})
	set_target_properties(${target} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/test"
	)
	target_include_directories(${target} PRIVATE
		"${CMAKE_SOURCE_DIR}/include"
		"${CMAKE_SOURCE_DIR}/test/unity"
	)
	target_compile_options(${target} PRIVATE -Wall -Wextra -g)
	add_test(NAME ${target} COMMAND ${target})
endfunction()

add_custom_target(tests DEPENDS
	test_example
	test_sbrk_sbrkAlloc
	test_sbrk_sbrkFree
	test_sbrk_sbrkRealloc
	test_arena_arenaCreate
	test_arena_arenaAlloc
	test_arena_arenaReset
	test_arena_arenaDestroy
	test_pool_poolCreate
	test_pool_poolAlloc
	test_pool_poolFree
	test_pool_poolDestroy
)

add_unity_test(test_example test/test_example.c)
add_unity_test(test_sbrk_sbrkAlloc test/alloc/test_sbrkAlloc.c)
add_unity_test(test_sbrk_sbrkFree test/alloc/test_sbrkFree.c)
add_unity_test(test_sbrk_sbrkRealloc test/alloc/test_sbrkRealloc.c)
add_unity_test(test_arena_arenaCreate test/arena/test_arenaCreate.c)
add_unity_test(test_arena_arenaAlloc test/arena/test_arenaAlloc.c)
add_unity_test(test_arena_arenaReset test/arena/test_arenaReset.c)
add_unity_test(test_arena_arenaDestroy test/arena/test_arenaDestroy.c)
add_unity_test(test_pool_poolCreate test/pool/test_poolCreate.c)
add_unity_test(test_pool_poolAlloc test/pool/test_poolAlloc.c)
add_unity_test(test_pool_poolFree test/pool/test_poolFree.c)
add_unity_test(test_pool_poolDestroy test/pool/test_poolDestroy.c)
