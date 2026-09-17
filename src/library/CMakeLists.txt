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
# Configure the public header for the BigWhoop library     #
#----------------------------------------------------------#
add_library(bwclib bitstream.c
                   libbwc.c
                   codestream.c
                   dwt.c
                   mq.c
                   tier1.c
                   tier2.c
                   tagtree.c)

#----------------------------------------------------------#
# Set the target compile definition for the encoder/decod- #
# er bit precision.                                        #
#----------------------------------------------------------#
if("${BIGWHOOP_PRECISION}" STREQUAL "Single")
   target_compile_definitions(bwclib PRIVATE -DBWC_SINGLE_PRECISION)
   string(CONCAT BWC_PRECISION_TYPE_BLOCK "typedef float                 bwc_float;\n  "
                                          "typedef uint32                bwc_raw;")
   string(CONCAT BWC_PRECISION_MACRO_BLOCK "#define PREC_BIT              31\n  "
                                           "#define MAXIMUM_NO_PASSES     (32 * 3) - 2")
else()
   target_compile_definitions(bwclib PRIVATE -DBWC_DOUBLE_PRECISION)
   string(CONCAT BWC_PRECISION_TYPE_BLOCK "typedef double                bwc_float;\n  "
                                          "typedef uint64                bwc_raw;")
   string(CONCAT BWC_PRECISION_MACRO_BLOCK "#define PREC_BIT              63\n  "
                                           "#define MAXIMUM_NO_PASSES     (64 * 3) - 2")
endif()

configure_file(${PROJECT_SOURCE_DIR}/include/library/public/bwc.h.in
               ${PROJECT_BINARY_DIR}/include/library/public/bwc.h
               @ONLY)

#----------------------------------------------------------#
# Set the target compile definition for the profiling      #
# output.                                                  #
#----------------------------------------------------------#
if(${BIGWHOOP_WITH_PROFILING})
  target_compile_definitions(bwclib PRIVATE -DBWC_PROFILE)
  MESSAGE(STATUS "Profiling: ${PROF}")
endif()

#----------------------------------------------------------#
# Set the Version and SOVersion and define the public API  # 
# for the BigWhoop library.                                #
#----------------------------------------------------------#
set_target_properties(bwclib PROPERTIES VERSION "${BWC_VERSION}"
                                      SOVERSION "${BWC_VERSION_MAJOR}"
                                  PUBLIC_HEADER "${PROJECT_BINARY_DIR}/include/library/public/bwc.h")

#----------------------------------------------------------#
# Setup up the include directory for the BigWhoop library. #
#----------------------------------------------------------#
target_include_directories(bwclib PRIVATE ${PROJECT_BINARY_DIR}/include/library/private
                                          ${PROJECT_SOURCE_DIR}/include/library/private
                                   PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/library/public>
                                          $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_include_directories(bwclib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

#----------------------------------------------------------#
# Link the BigWhoop library to the math.h library.         #
# With MSVC Math functions are built into the C runtime,   #
# no separate linking needed                               #
#----------------------------------------------------------#
if(NOT MSVC)
  target_link_libraries(bwclib PRIVATE m)
endif()

#----------------------------------------------------------#
# Setup the install directories and target exporting for   #
# config-file packaging.                                   #
#----------------------------------------------------------#
install(      TARGETS bwclib
               EXPORT bwc-targets
              LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#----------------------------------------------------------#
# Define the output name for the BigWhoop library.         #
#----------------------------------------------------------#
set_property(TARGET bwclib PROPERTY OUTPUT_NAME bwc)