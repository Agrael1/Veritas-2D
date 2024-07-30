project(engine C)

# template compilation routine
add_custom_target(InvalidateTemplates ALL
                  COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_SOURCE_DIR}/src/templates.c")

add_library (${PROJECT_NAME} STATIC
    "src/templates.c"

    "src/keyboard.c"
    "src/mouse.c"
    "src/control.c"
    "src/swapchain.c"
    "src/window.c"
    "src/veritas.c"

    "include/engine/control.h"
    "include/engine/win_setup.h"
    "include/engine/keyboard_event.h"
    "include/engine/mouse_event.h"
    "include/engine/swapchain.h"
    "include/engine/keyboard.h"
    "include/engine/mouse.h"
    "include/engine/window.h"
    "include/engine/veritas.h"
)
add_library(ver::engine ALIAS ${PROJECT_NAME})

target_link_libraries("${PROJECT_NAME}" PUBLIC ver::vlib)
target_include_directories("${PROJECT_NAME}" PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 17)

# Invalidate templates on every build
add_dependencies(${PROJECT_NAME} InvalidateTemplates)

