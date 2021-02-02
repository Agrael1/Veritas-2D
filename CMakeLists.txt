# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)

project ("ConsoleApplication5")

# Включите подпроекты.
add_subdirectory ("Vlib")
add_subdirectory ("VEngine")
add_subdirectory ("BadApple")
