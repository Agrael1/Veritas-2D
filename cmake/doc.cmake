# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile CACHE STRING "Doxygen configuration file")
    set(DOXYGEN_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doc CACHE STRING "Doxygen output directory")

    file(MAKE_DIRECTORY ${DOXYGEN_OUT_DIR})

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)


function(generate_doc TARGET)
    if(NOT DOXYGEN_FOUND OR NOT BUILD_DOC)
		return()
    endif()

	add_custom_target(doc_${TARGET}
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
		WORKING_DIRECTORY ${DOXYGEN_OUT_DIR}
		COMMENT "Generating API documentation with Doxygen"
		VERBATIM )

    add_dependencies(${TARGET} doc_${TARGET})
endfunction()