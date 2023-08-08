cmake_minimum_required(VERSION 3.22)

get_filename_component(ROOT ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${ROOT} C)

set(CMAKE_GENERATOR Ninja)

# update submodules
add_custom_target(update_dependencies
	COMMAND ${PROXY} git submodule foreach git pull
	WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

# c compiler settings
set(CMAKE_C_STANDARD 99)

# strip
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")

if(MSVC)
	# add_compile_options(/fsanitize=address)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:AVX2 /Ox /GL /fp:strict /MT")
	add_compile_options(/Zc:__cplusplus) # see https://docs.microsoft.com/en-us/cpp/build/reference/zc-cplusplus?view=msvc-160 for detail
else()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_REENTRANT -mavx2 -mbmi2 -Ofast -flto")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_program(GIT_CHGLOG git-chglog)
if(GIT_CHGLOG)
	message(STATUS "Found git-chglog:${GIT_CHGLOG}")
	execute_process(
		COMMAND ${GIT_CHGLOG}
		OUTPUT_FILE CHANGELOG.md
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
else()
	message(STATUS "git-chglog not found, changelog will not be updated")
endif()

enable_testing()

set(CUNIT_DISABLE_TESTS TRUE)
set(CUNIT_DISABLE_EXAMPLES TRUE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/cunit)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/cunit/CUnit)
set(SDL_STATIC TRUE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL/include)
link_libraries(SDL2-static)

add_subdirectory(src)
add_subdirectory(test)
