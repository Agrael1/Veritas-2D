cmake_minimum_required (VERSION 3.8)

project(Maze C)
set(C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB hdrs "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Добавьте источник в исполняемый файл этого проекта.
add_executable ("${PROJECT_NAME}" ${srcs} ${hdrs})

target_link_libraries("${PROJECT_NAME}" PUBLIC VEngine)
target_include_directories("${PROJECT_NAME}" PRIVATE 
			"${CMAKE_CURRENT_SOURCE_DIR}/include"
)