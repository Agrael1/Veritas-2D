# CMakeList.txt: проект CMake для Vlib; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project(Vlib C)

# Добавьте источник в исполняемый файл этого проекта.
add_library (Vlib STATIC 
	"src/VString.c"

	"include/RuntimeClass.h"
	"include/VString.h"
	 "include/VHashMapT.h" "include/VHashMapT.c" "include/VRingT.h" "include/VOptionalT.h" "include/VOptionalT.c" "include/VRingT.c" "include/VBitSet.h")

target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
