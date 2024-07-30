cmake_minimum_required (VERSION 3.15)

project ("Veritas")

option(BUILD_DOC "Build documentation" OFF)
option(BUILD_TESTS "Build tests" OFF)


set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
include(FetchContent)
include(cmake/doc.cmake)
include(cmake/tests.cmake)


add_subdirectory ("vlib")
add_subdirectory ("vmath")
add_subdirectory ("engine")
add_subdirectory ("projects")
