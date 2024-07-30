project(vmath LANGUAGES C)

add_library(${PROJECT_NAME} STATIC
		"src/vmath.c"

		"include/vmath/constants.h"
		"include/vmath/math_types.h"
		"include/vmath/vmint.h"
		"include/vmath/vmstore_load.h"
		"include/vmath/vmvector.h"
		"include/vmath/vmvector3.h"
		"include/vmath/vmplane.h"
		"include/vmath/vmath.h"
)
add_library(ver::vmath ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 17)
