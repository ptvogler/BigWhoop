#*================================================================================================*#
#|                                                                                                |#
#|       /$$$$$$$  /$$                  /$$      /$$ /$$                                          |#
#|      | $$__  $$|__/                 | $$  /$ | $$| $$                                          |#
#|      | $$  \ $$ /$$  /$$$$$$        | $$ /$$$| $$| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$       |#
#|      | $$$$$$$ | $$ /$$__  $$       | $$/$$ $$ $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$      |#
#|      | $$__  $$| $$| $$  \ $$       | $$$$_  $$$$| $$  \ $$| $$  \ $$| $$  \ $$| $$  \ $$      |#
#|      | $$  \ $$| $$| $$  | $$       | $$$/ \  $$$| $$  | $$| $$  | $$| $$  | $$| $$  | $$      |#
#|      | $$$$$$$/| $$|  $$$$$$$       | $$/   \  $$| $$  | $$|  $$$$$$/|  $$$$$$/| $$$$$$$/      |#
#|      |_______/ |__/ \____  $$       |__/     \__/|__/  |__/ \______/  \______/ | $$____/       |#
#|                     /$$  \ $$                                                  | $$            |#
#|                    |  $$$$$$/                                                  | $$            |#
#|                     \______/                                                   |__/            |#
#|                                                                                                |#
#|  DESCRIPTION:                                                                                  |#
#|  ------------                                                                                  |#
#|                                                                                                |#
#|        Defines the global cmake script for the Big Whoop compression algorithm.                |#
#|                                                                                                |#
#|  --------------------------------------------------------------------------------------------  |#
#|  Copyright (c) 2023, High Performance Computing Center - University of Stuttgart               |#
#|                                                                                                |#
#|  Redistribution and use in source and binary forms, with or without modification, are          |#
#|  permitted provided that the following conditions are met:                                     |#
#|                                                                                                |#
#|     (1)   Redistributions of source code must retain the above copyright notice, this list of  |#
#|           conditions and the following disclaimer.                                             |#
#|                                                                                                |#
#|     (2)   Redistributions in binary form must reproduce the above copyright notice, this list  |#
#|           of conditions and the following disclaimer in the documentation and/or other         |#
#|           materials provided with the distribution.                                            |#
#|                                                                                                |#
#|  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS   |#
#|  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF               |#
#|  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE    |#
#|  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,     |#
#|  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF            |#
#|  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)        |#
#|  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR      |#
#|  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  |#
#|  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                            |#
#|                                                                                                |#
#*================================================================================================*#
#----------------------------------------------------------#
# Check if the version requirement for cmake is met.       #
#----------------------------------------------------------#
cmake_minimum_required(VERSION 3.5.1)

#----------------------------------------------------------#
# Set the project name and description.                    #
#----------------------------------------------------------#
project(BWC LANGUAGES C)
set(PROJECT_DESCRIPTION "Compression algorithm for IEEE 754 datasets")

#----------------------------------------------------------#
# Check that the current build is not a in-source build.   #
#----------------------------------------------------------#
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
   message(FATAL_ERROR "In-source builds are prohibited. Please create a separate build directory")
endif()

#----------------------------------------------------------#
# Save the bwc.c file in a temporary variable and match    #
# the current version number with a regular expression.    #
#----------------------------------------------------------#
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/include/library/private/macros.h _macros_h_contents)
string(REGEX REPLACE ".*#define[ \t]+BWC_VERSION_MAJOR[ \t]+([0-9]+).*"
     "\\1" BWC_VERSION_MAJOR ${_macros_h_contents})
string(REGEX REPLACE ".*#define[ \t]+BWC_VERSION_MINOR[ \t]+([0-9]+).*"
     "\\1" BWC_VERSION_MINOR ${_macros_h_contents})
string(REGEX REPLACE ".*#define[ \t]+BWC_VERSION_PATCH[ \t]+([0-9]+).*"
     "\\1" BWC_VERSION_PATCH ${_macros_h_contents})
set(BWC_VERSION "${BWC_VERSION_MAJOR}.${BWC_VERSION_MINOR}.${BWC_VERSION_PATCH}")

#----------------------------------------------------------#
# Setup a user-specified option used to control the sample #
# precision during compression. The standard option is     #
# set to double precision.                                 #
#----------------------------------------------------------#
set(PREC "Double" CACHE STRING "User-specified option used to control\
                                the precision during compression")

#----------------------------------------------------------#
# Inlude the GNU standard installation directories module  #
# to set up the output directories.                        #
#----------------------------------------------------------#
include(GNUInstallDirs)

#----------------------------------------------------------#
# Set up the output directories for the Big Whoop library  #
# and utility binaries.                                    #
#----------------------------------------------------------#
list(INSERT CMAKE_MODULE_PATH 0 "${BWC_SOURCE_DIR}/cmake")
if(${TOOL})
  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BWC_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
  endif()
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BWC_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
endif()
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BWC_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
endif()

#----------------------------------------------------------#
# Set the target installation directory of the config-file #
# packaging configuration files.                           #
#----------------------------------------------------------#
set(CMAKE_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/bwc
    CACHE PATH "Installation directory for config-file package cmake files")

#----------------------------------------------------------#
# Suggest the C standard (C99) used by the compiler.       #
#----------------------------------------------------------#
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 11)
endif()

message(STATUS "Compiling with C standard: ${CMAKE_C_STANDARD}")

#----------------------------------------------------------#
# Suggest the C++ standard (CXX98) used by the compiler.   #
#----------------------------------------------------------#
# Suggest C++98
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11)
endif()

message(STATUS "Compiling with C++ standard: ${CMAKE_CXX_STANDARD}")

#----------------------------------------------------------#
# Check if the OpenMP package is available for the current #
# setup and set the appropriate C/C++ flags.               #
#----------------------------------------------------------#
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR "${OMP}")
  message(STATUS "Enable OpenMP parallelization")
 find_package(OpenMP)
 if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
 endif()
endif()

#----------------------------------------------------------#
# Add all necessary compiler warnings for debugging.       #
#----------------------------------------------------------#
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(CMAKE_C_FLAGS    "${CMAKE_C_FLAGS} -Wall -Wextra")
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

#----------------------------------------------------------#
# Include the CMake dependent option macro.                #
#----------------------------------------------------------#
include(CMakeDependentOption)

#----------------------------------------------------------#
# Set the -fPIC compile option for static libraries.       #
#----------------------------------------------------------#
get_property(SHARED_LIBS_SUPPORTED GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS)
cmake_dependent_option(BUILD_SHARED_LIBS
  "Whether or not to build shared libraries" ON
  "SHARED_LIBS_SUPPORTED" OFF)

if(SHARED_LIBS_SUPPORTED)
  cmake_dependent_option(BWC_ENABLE_PIC
    "Build with Position Independent Code" ON
    "NOT BUILD_SHARED_LIBS" ON)
endif()

if(BWC_ENABLE_PIC)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

#----------------------------------------------------------#
# Add the project source code.                             #
#----------------------------------------------------------#
add_subdirectory(src/library)

#----------------------------------------------------------#
# Add the utilities source code if requested.              #
#----------------------------------------------------------#
if(${TOOL})
  add_subdirectory(src/tools)
else()
  set(ignoreMe "${BUILD_EAS3}${BUILD_NETCDF}")
endif()

#----------------------------------------------------------#
# Config-file packaging                                    #
#----------------------------------------------------------#
export(TARGETS bwclib NAMESPACE bwc:: FILE "bwc-targets.cmake")
configure_file(${PROJECT_SOURCE_DIR}/bwc-config.cmake.in
              "${PROJECT_BINARY_DIR}/bwc-config.cmake" @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/bwc-config-version.cmake.in
              "${PROJECT_BINARY_DIR}/bwc-config-version.cmake" @ONLY)
  
install(      FILES "${PROJECT_BINARY_DIR}/bwc-config.cmake"
                    "${PROJECT_BINARY_DIR}/bwc-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_CMAKEDIR})

install(     EXPORT bwc-targets
          NAMESPACE bwc::
        DESTINATION ${CMAKE_INSTALL_CMAKEDIR})