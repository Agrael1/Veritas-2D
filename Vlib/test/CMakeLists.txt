cmake_minimum_required (VERSION 3.15)
project(Tests_Vlib CXX)

include_directories("${googletest_SOURCE_DIR}/googletest/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}../include")
include(GoogleTest)

add_executable(
  "${PROJECT_NAME}"
  "test_String.cpp"
)
target_link_libraries(
	"${PROJECT_NAME}" PUBLIC
	Vlib
	gtest_main
)

gtest_discover_tests("${PROJECT_NAME}")