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
#|        Defines the cmake script for the BigWhoop command line tool.                            |#
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
# Add the bwc command line utility and get hash tool to    #
# the current project using the utility source files.      #
#----------------------------------------------------------#
add_executable(bwccmd  bwccmdl.c
                       ../interfaces/reader/eas3.c)

#----------------------------------------------------------#
# Set the target compile definition for the requested file #
# format support.                                          #
#----------------------------------------------------------#
if(${BUILD_EAS3})
  target_compile_definitions(bwccmd PRIVATE -DBWC_EAS3)
  MESSAGE(STATUS "EAS3 file format support: ${BUILD_EAS3}")
endif()

if(${BUILD_NETCDF})
  target_compile_definitions(bwccmd PRIVATE -DBWC_NETCDF)
  MESSAGE(STATUS "NetCDF file format support: ${BUILD_NETCDF}")
endif()

#----------------------------------------------------------#
# Define the output name for the utility binaries.         #
#----------------------------------------------------------#
set_property(TARGET bwccmd  PROPERTY OUTPUT_NAME bwc)

#----------------------------------------------------------#
# Setup up the include directory for the bwc utilities.    #
#----------------------------------------------------------#
target_include_directories(bwccmd PRIVATE ${CMAKE_SOURCE_DIR}/include/tools)
target_include_directories(bwccmd PRIVATE ${CMAKE_SOURCE_DIR}/include/library/public)
target_include_directories(bwccmd PRIVATE ${CMAKE_SOURCE_DIR}/include/interfaces/reader)

#----------------------------------------------------------#
# Setup the install directories.                           #
#----------------------------------------------------------#
install(TARGETS bwccmd DESTINATION ${CMAKE_INSTALL_BINDIR})

#----------------------------------------------------------#
# Link the bwc utility to the bwc library.                 #
#----------------------------------------------------------#
target_link_libraries(bwccmd PRIVATE bwclib m)