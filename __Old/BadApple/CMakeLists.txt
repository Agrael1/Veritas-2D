# CMakeList.txt: ������ CMake ��� Vlib; �������� �������� ��� � �����������,
# ������� ����� ������ ��� ����������� �������.
#
cmake_minimum_required (VERSION 3.8)

project(BadApple C)
set(C_STANDARD 11)
if (MSVC) 
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /std:c11") 
endif ()

# �������� �������� � ����������� ���� ����� �������.
add_executable ("${PROJECT_NAME}" 
"src/EntryMain.c"
  )

target_link_libraries("${PROJECT_NAME}" PUBLIC VEngine MediaPlayer)
target_include_directories("${PROJECT_NAME}" PRIVATE 
			"${CMAKE_CURRENT_SOURCE_DIR}/include"
			"${CMAKE_SOURCE_DIR}/MediaPlayer/include")
# TODO: �������� ����� � ������� �������, ���� ��� ����������.