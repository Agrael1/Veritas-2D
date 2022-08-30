# CMakeList.txt: проект CMake для Vlib; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project(BadApple C)
set(C_STANDARD 11)
if (MSVC) 
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /std:c11") 
endif ()

# Добавьте источник в исполняемый файл этого проекта.
add_executable ("${PROJECT_NAME}" 
"src/EntryMain.c"
   "../Maze/include/Maze.h")

target_link_libraries("${PROJECT_NAME}" PUBLIC VEngine MediaPlayer)
target_include_directories("${PROJECT_NAME}" PRIVATE 
			"${CMAKE_CURRENT_SOURCE_DIR}/include"
			"${CMAKE_SOURCE_DIR}/MediaPlayer/include")
# TODO: Добавьте тесты и целевые объекты, если это необходимо.