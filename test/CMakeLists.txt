file(GLOB srcs *_test.c)
foreach(src ${srcs})
	get_filename_component(test_name ${src} NAME_WLE)
	add_executable(${test_name} ${src})
	add_test(${test_name}_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${test_name})
	target_link_libraries(${test_name} cunit)
endforeach()
