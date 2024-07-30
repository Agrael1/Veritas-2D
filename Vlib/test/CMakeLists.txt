project("vlib_test" CXX)

add_executable(${PROJECT_NAME}
	"test_String.cpp"
	"test_VRing.cpp" 
	"test_VHashMap.cpp"
)

target_link_libraries(
	"${PROJECT_NAME}" PRIVATE
	ver::vlib
	Catch2::Catch2WithMain
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

catch_discover_tests(${PROJECT_NAME} TEST_PREFIX "ver.${PROJECT_NAME}.")