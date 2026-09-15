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
#|        Defines the cmake script for the libbwc library.                                        |#
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
# Define the BigWhoop object library.                      #
#----------------------------------------------------------#
add_library(bwc_objects OBJECT bitstream.c
                               libbwc.c
                               codestream.c
                               dwt.c
                               mq.c
                               tier1.c
                               tier2.c
                               tagtree.c)

#----------------------------------------------------------#
# Configure compression precision and compile time defin.  #
#----------------------------------------------------------#
if("${BIGWHOOP_PRECISION}" STREQUAL "Single")
 target_compile_definitions(bwc_objects PRIVATE -DBWC_SINGLE_PRECISION)
 string(CONCAT BWC_PRECISION_TYPE_BLOCK  "typedef float                 bwc_float;\n  "
                                         "typedef uint32                bwc_raw;")
 string(CONCAT BWC_PRECISION_MACRO_BLOCK "#define PREC_BIT              31\n  "
                                         "#define MAXIMUM_NO_PASSES     (32 * 3) - 2")
else()
 target_compile_definitions(bwc_objects PRIVATE -DBWC_DOUBLE_PRECISION)
 string(CONCAT BWC_PRECISION_TYPE_BLOCK  "typedef double                bwc_float;\n  "
                                         "typedef uint64                bwc_raw;")
 string(CONCAT BWC_PRECISION_MACRO_BLOCK "#define PREC_BIT              63\n  "
                                         "#define MAXIMUM_NO_PASSES     (64 * 3) - 2")
endif()

if(BWC_WITH_PROF)
  target_compile_definitions(bwc_objects PRIVATE BWC_PROFILE)
  message(STATUS "Profiling enabled for library")
endif()

#----------------------------------------------------------#
# Link the C OpenMP target.                                #
#----------------------------------------------------------#
if(BWC_WITH_OMP AND OpenMP_C_FOUND)
  target_link_libraries(bwc_objects PRIVATE OpenMP::OpenMP_C)
endif()

#----------------------------------------------------------#
# Configure BigWhoop header files.                         #
#----------------------------------------------------------#
configure_file(${PROJECT_SOURCE_DIR}/include/library/public/bwc.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/include/library/public/bwc.h
               @ONLY)

configure_file(${PROJECT_SOURCE_DIR}/include/library/private/macros.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/include/library/private/macros.h
               @ONLY)

#----------------------------------------------------------#
# Setup up the include directory for the BigWhoop library. #
#----------------------------------------------------------#
target_include_directories(bwc_objects
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include/library/public>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/bwc>
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include/library/private
    ${CMAKE_CURRENT_BINARY_DIR}/include/library/private)

#----------------------------------------------------------#
# Create the shared library target.                        #
#----------------------------------------------------------#
if(BWC_BUILD_SHARED OR BWC_BUILD_UTIL)
  add_library(bwclib $<TARGET_OBJECTS:bwc_objects>)

  set_target_properties(bwclib PROPERTIES
      PUBLIC_HEADER "${CMAKE_CURRENT_BINARY_DIR}/include/library/public/bwc.h"
      VERSION ${PROJECT_VERSION}
      SOVERSION ${PROJECT_VERSION_MAJOR}
      OUTPUT_NAME "bwc")

  # Link the math library (only if not MSVC)
  if(NOT MSVC)
    target_link_libraries(bwclib PRIVATE m)
  endif()

  # Propagate properties from objects to the library
  target_link_libraries(bwclib PUBLIC bwc_objects)

  # Installation
  install(TARGETS bwclib
      EXPORT bwc-targets
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/bwc)
endif()

#----------------------------------------------------------#
# Create the static library target.                        #
#----------------------------------------------------------#
if(BWC_BUILD_STATIC OR (BWC_BUILD_HDF5 AND NOT BWC_BUILD_SHARED))
  add_library(bwclib_static STATIC $<TARGET_OBJECTS:bwc_objects>)

  set_target_properties(bwclib_static PROPERTIES
      PUBLIC_HEADER "${CMAKE_CURRENT_BINARY_DIR}/include/library/public/bwc.h"
      VERSION ${PROJECT_VERSION}
      OUTPUT_NAME "bwc-static")

  if(NOT MSVC)
    target_link_libraries(bwclib_static PRIVATE m)
  endif()

  target_link_libraries(bwclib_static PUBLIC bwc_objects)

  install(TARGETS bwclib_static
      EXPORT bwc-targets
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/bwc)
endif()

#----------------------------------------------------------#
# Export targets for packaging.                            #
#----------------------------------------------------------#
if(TARGET bwclib)
  add_library(bwc::bwclib ALIAS bwclib)
endif()

if(TARGET bwclib_static)
  add_library(bwc::bwclib_static ALIAS bwclib_static)
endif()