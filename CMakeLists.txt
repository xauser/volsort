# CMake confiquration file of volsort
#
# The code was debugged against gcc 4.3.2
# using:
#
# CFLAGS="-std=c99 -D_GNU_SOURCE -pedantic -Wall"
# set(CMAKE_VERBOSE_MAKEFILE 1)
#
cmake_minimum_required(VERSION 2.4)
project(VOLSORT C)
set(VOLSORT_VERSION_MAJOR 1)
set(VOLSORT_VERSION_MINOR 0)
set(VOLSORT_VERSION_PATCH 2)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h)
file(GLOB_RECURSE VOLSORT_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_executable(volsort ${VOLSORT_SOURCE_FILES})
