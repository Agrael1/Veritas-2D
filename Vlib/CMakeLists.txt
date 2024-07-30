project("vlib" C)

add_library (${PROJECT_NAME} STATIC 
	"include/vlib/string.h"
	"include/vlib/hash_map_t.h"
	"include/vlib/hash_map_t.c" 
	"include/vlib/ring_t.h" 
	"include/vlib/ring_t.c" 
	"include/vlib/optional_t.h" 
	"include/vlib/optional_t.c" 
	"include/vlib/bitset.h" 

	"include/vlib/runtime.h"
	
	"src/string.c"
	"src/bitset.c"
)
add_library(ver::vlib ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 17)
target_include_directories(${PROJECT_NAME} 
PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_definitions(${PROJECT_NAME} PUBLIC CLEANUP_SUPPORTED=0)
else()
	target_compile_definitions(${PROJECT_NAME} PUBLIC CLEANUP_SUPPORTED=1)
endif()

generate_doc(${PROJECT_NAME})

#tests
if (BUILD_TESTS)
	add_subdirectory ("test")
endif()