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

# Each test needs to be appended below
add_custom_target(tests DEPENDS test_example test_alloc_myAlloc test_alloc_myFree test_alloc_myRealloc)

# Begin test block; copy this block for each test file
add_executable(test_example
	test/test_example.c
	${UNITY_SRC}
	${test_sources}
)
set_target_properties(test_example PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/test"
)
target_include_directories(test_example PRIVATE
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/test/unity"
)
target_compile_options(test_example PRIVATE -Wall -Wextra -g)

add_test(NAME example COMMAND test_example)
# End test block.

add_executable(test_alloc_myAlloc
	test/alloc/test_myAlloc.c
	${UNITY_SRC}
	${test_sources}
)
set_target_properties(test_alloc_myAlloc PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/test"
)
target_include_directories(test_alloc_myAlloc PRIVATE
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/test/unity"
)
target_compile_options(test_alloc_myAlloc PRIVATE -Wall -Wextra -g)

add_test(NAME alloc COMMAND test_alloc_myAlloc)

add_executable(test_alloc_myFree
	test/alloc/test_myFree.c
	${UNITY_SRC}
	${test_sources}
)
set_target_properties(test_alloc_myFree PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/test"
)
target_include_directories(test_alloc_myFree PRIVATE
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/test/unity"
)
target_compile_options(test_alloc_myFree PRIVATE -Wall -Wextra -g)

add_test(NAME myFree COMMAND test_alloc_myFree)

add_executable(test_alloc_myRealloc
	test/alloc/test_myRealloc.c
	${UNITY_SRC}
	${test_sources}
)
set_target_properties(test_alloc_myRealloc PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/test"
)
target_include_directories(test_alloc_myRealloc PRIVATE
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/test/unity"
)
target_compile_options(test_alloc_myRealloc PRIVATE -Wall -Wextra -g)

add_test(NAME myRealloc COMMAND test_alloc_myRealloc)
