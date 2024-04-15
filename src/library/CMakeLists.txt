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
# Assemble the public header for the BigWhoop library      #
#----------------------------------------------------------#
set(PYTHON_ARGUMENT "")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  if(OPENMP_FOUND)
    set(PYTHON_ARGUMENT ${PYTHON_ARGUMENT} -OMP)
  endif()
endif()

if("${PREC}" STREQUAL "Single")
  set(PYTHON_ARGUMENT ${PYTHON_ARGUMENT} -Single)
endif()

execute_process(COMMAND python3 public_header.py ${PYTHON_ARGUMENT}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE result
                ERROR_VARIABLE error_output)

if(result)
  message(WARNING "Public header generation failed: ${error_output}")
endif()

#----------------------------------------------------------#
# Set the linking type according to user choice and add    #
# the library to the current project                       #
#----------------------------------------------------------#
if("${LINK}" STREQUAL "Static")
  set(BWC_LINK "STATIC")
else()
  set(BWC_LINK "SHARED")
endif()

add_library(bwclib ${BWC_LINK} bitstream.c
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
if("${PREC}" STREQUAL "Single")
   target_compile_definitions(bwclib PRIVATE -DBWC_SINGLE_PRECISION)
else()
   target_compile_definitions(bwclib PRIVATE -DBWC_DOUBLE_PRECISION)
endif()

#----------------------------------------------------------#
# Set the Version and SOVersion and define the public API  # 
# for the BigWhoop library.                                #
#----------------------------------------------------------#
set_target_properties(bwclib PROPERTIES VERSION ${BWC_VERSION}
                                      SOVERSION ${BWC_VERSION_MAJOR}
                                  PUBLIC_HEADER ${CMAKE_SOURCE_DIR}/include/library/public/bwc.h)

#----------------------------------------------------------#
# Setup up the include directory for the BigWhoop library. #
#----------------------------------------------------------#
target_include_directories(bwclib PRIVATE ${CMAKE_SOURCE_DIR}/include/library/private
                                   PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/library/public>
                                          $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

#----------------------------------------------------------#
# Link the BigWhoop library to the math.h library.         #
#----------------------------------------------------------#
target_link_libraries(bwclib PRIVATE m)

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