# CMakeList.txt: проект CMake для Vlib; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project(VEngine C)


set(CMAKE_C_STANDARD 17)
if (MSVC) 
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /std:c17") 
endif ()

add_custom_target(InvalidateTemplates ALL
                  COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_SOURCE_DIR}/src/Templates.c")

# Добавьте источник в исполняемый файл этого проекта.
add_library ("${PROJECT_NAME}" STATIC 
"src/Templates.c"

"include/Templates.h"
"include/WinSetup.h"
 "include/Keyboard.h" 
 "src/Keyboard.c" 
 "include/Mouse.h" 
 "src/Mouse.c" 
 "include/Control.h" 
 "src/Control.c" 
 "include/Window.h" 
 "src/Window.c" 
 "include/SwapChain.h" 
 "src/SwapChain.c" 
 "include/VeritasEngine.h" 
 "src/VeritasEngine.c")

target_link_libraries("${PROJECT_NAME}" PUBLIC Vlib)
target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
