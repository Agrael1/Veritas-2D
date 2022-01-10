cmake_minimum_required (VERSION 3.15)

project(MediaPlayer CXX)

set(NUGET "${CMAKE_SOURCE_DIR}/NuGet/nuget.exe")

FetchContent_Declare(
  wil
  GIT_REPOSITORY https://github.com/microsoft/wil.git
  GIT_TAG origin/master
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)
FetchContent_GetProperties(wil)
if(NOT wil_POPULATED)
  FetchContent_Populate(wil)
endif()

# Because we don't always use msbuild, we need to run nuget manually
find_program(NUGET nuget)
if (NOT NUGET)
    message(FATAL_ERROR "Unable to find the nuget CLI tool. Please install it from https://www.nuget.org/downloads and ensure it has been added to the NuGet directory of project")
endif()

execute_process(COMMAND
    ${NUGET} install Microsoft.Windows.CppWinRT -OutputDirectory packages
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    RESULT_VARIABLE ret)
if (NOT ret EQUAL 0)
    message(FATAL_ERROR "Failed to install nuget package Microsoft.Windows.CppWinRT")
endif()

file(GLOB CPPWINRT_DIR ${CMAKE_BINARY_DIR}/packages/Microsoft.Windows.CppWinRT.*)

set(CPPWINRT ${CPPWINRT_DIR}/bin/cppwinrt.exe)
execute_process(COMMAND
    ${CPPWINRT} -input sdk -output include
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    RESULT_VARIABLE ret)
if (NOT ret EQUAL 0)
    message(FATAL_ERROR "Failed to run cppwinrt.exe")
endif()

include_directories(BEFORE SYSTEM ${CMAKE_BINARY_DIR}/include)

## first we can indicate the documentation build as an option and set it to ON by default
#option(BUILD_DOC "Build documentation" ON)
#
## check if Doxygen is installed
#find_package(Doxygen)
#if (DOXYGEN_FOUND)
#    # set input and output files
#    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
#    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
#
#    # request to configure the file
#    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
#    message("Doxygen build started")
#
#    string(CONCAT MULTI "${PROJECT_NAME}" "_doc")
#
#    # note the option ALL which allows to build the docs together with the application
#    add_custom_target( "${MULTI}" ALL
#        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
#        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#        COMMENT "Generating API documentation with Doxygen"
#        VERBATIM )
#else (DOXYGEN_FOUND)
#  message("Doxygen need to be installed to generate the doxygen documentation")
#endif (DOXYGEN_FOUND)

add_library ("${PROJECT_NAME}" STATIC 
    "src/MediaEngine.cpp"
    "include/MediaEngine.h"
    "include/MPAPI.h"
)

target_link_libraries("${PROJECT_NAME}" PUBLIC "d3d11.lib" "Mfplat.lib")
target_include_directories("${PROJECT_NAME}" PUBLIC "${wil_SOURCE_DIR}/include")
target_include_directories("${PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/include" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
