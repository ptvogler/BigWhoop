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
cmake_minimum_required(VERSION 3.16...4.3.0)

#----------------------------------------------------------#
# Check that the current build is not a in-source build.   #
#----------------------------------------------------------#
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed. "
                      "Please create a separate build directory.\n"
                      "Example:\n"
                      "  mkdir build && cd build\n"
                      "  cmake ..")
endif()

#----------------------------------------------------------#
# Set the project name and description.                    #
#----------------------------------------------------------#
project(BWC 
        VERSION 0.2.4
        LANGUAGES C CXX
        DESCRIPTION "Compression algorithm for IEEE 754 datasets")

#----------------------------------------------------------#
# Set the project options.                                 #
#----------------------------------------------------------#
set(BIGWHOOP_PRECISION "Double" CACHE STRING "Precision mode")
set_property(CACHE BIGWHOOP_PRECISION PROPERTY STRINGS "Double" "Single")

option(BWC_BUILD_SHARED "Build shared libraries" ON)
option(BWC_BUILD_STATIC "Build static libraries" OFF)
option(BWC_BUILD_UTIL   "Build command line utilities" OFF)
option(BWC_BUILD_HDF5   "Build HDF5 plugin" OFF)
option(BWC_BUILD_TESTS  "Build unit tests" ON)

option(BWC_WITH_OMP  "Enable OpenMP parallelization" ON)
option(BWC_WITH_PROF "Enable profiling" OFF)
option(BWC_WITH_EAS3 "Enable EAS3 support" OFF)

#----------------------------------------------------------#
# Set standards.                                           #
#----------------------------------------------------------#
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#----------------------------------------------------------#
# Set global flags.                                        #
#----------------------------------------------------------#
add_compile_options(-Wall)

if(NOT MSVC)
  add_compile_options(-Wextra)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(MSVC)
    add_compile_options(/WX)
  else()
    add_compile_options(-Werror)
  endif()
endif()

#----------------------------------------------------------#
# Fetch system dependencies.                               #
#----------------------------------------------------------#
if(BWC_WITH_OMP AND CMAKE_BUILD_TYPE STREQUAL "Release")
  find_package(OpenMP REQUIRED COMPONENTS C)
  if(OpenMP_C_FOUND)
    message(STATUS "OpenMP found: enabling parallelization")
  else()
    message(WARNING "OpenMP requested but not found. Disabling parallelization.")
  endif()
endif()

if(BWC_BUILD_HDF5)
  find_package(HDF5 REQUIRED COMPONENTS C CXX)
endif()

#----------------------------------------------------------#
# Fetch third party dependencies.                          #
#----------------------------------------------------------#
if(BWC_BUILD_TESTS)
  include(cmake/ThirdParty.cmake)
endif()

#----------------------------------------------------------#
# Process the compression library.                         #
#----------------------------------------------------------#
add_subdirectory(src/library)

if(BWC_BUILD_UTIL)
  add_subdirectory(src/tools)
endif()

#----------------------------------------------------------#
# Process the HDF5 filter plugin.                          #
#----------------------------------------------------------#
if(BWC_BUILD_HDF5)
  add_subdirectory(src/interfaces/HDF5)
endif()

#----------------------------------------------------------#
# Add unit testing.                                        #
#----------------------------------------------------------#
if(BWC_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

#----------------------------------------------------------#
# Config-file packaging                                    #
#----------------------------------------------------------#
configure_file(${PROJECT_SOURCE_DIR}/bwc-config.cmake.in
              "${PROJECT_BINARY_DIR}/bwc-config.cmake" @ONLY)

message(THIS IS IA TEST::::: ${CMAKE_INSTALL_LIBDIR})

configure_file(${PROJECT_SOURCE_DIR}/bwc-config-version.cmake.in
              "${PROJECT_BINARY_DIR}/bwc-config-version.cmake" @ONLY)

install(      FILES "${PROJECT_BINARY_DIR}/bwc-config.cmake"
                    "${PROJECT_BINARY_DIR}/bwc-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_CMAKEDIR})

install(     EXPORT bwc-targets
               FILE bwc-targets.cmake
          NAMESPACE bwc::
        DESTINATION ${CMAKE_INSTALL_CMAKEDIR})