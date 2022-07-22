cmake_minimum_required (VERSION 3.15)

project(Vlib C)

# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    string(CONCAT MULTI "${PROJECT_NAME}" "_doc")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( "${MULTI}" ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)


add_library ("${PROJECT_NAME}" STATIC 
	"src/VString.c"
	

	"include/RuntimeClass.h"
	"include/VString.h"
	
	 "include/VHashMapT.h" "include/VHashMapT.c" 
	 "include/VRingT.h" "include/VOptionalT.h" "include/VOptionalT.c" "include/VRingT.c" 
	 "include/VBitSet.h" "src/VBitSet.c")

target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/include" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory ("test")
