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
add_executable(bwc-cli  bwc-clil.c
                        ../interfaces/reader/eas3.c)

#----------------------------------------------------------#
# Define the output name for the utility binaries.         #
#----------------------------------------------------------#
set_property(TARGET bwc-cli  PROPERTY OUTPUT_NAME bwc-cli)

#----------------------------------------------------------#
# Setup up the include directory for the bwc utilities.    #
#----------------------------------------------------------#
target_include_directories(bwc-cli PRIVATE
  ${CMAKE_SOURCE_DIR}/include/tools
  ${CMAKE_SOURCE_DIR}/include/interfaces/reader)

#----------------------------------------------------------#
# Link the utility to the bwc library and dependencies.    #
#----------------------------------------------------------#
if(TARGET bwc::bwclib)
  target_link_libraries(bwc-cli PRIVATE bwc::bwclib)
elseif(TARGET bwc::bwclib_static)
  target_link_libraries(bwc-cli PRIVATE bwc::bwclib_static)
else()
  message(FATAL_ERROR "bwc library target not found.")
endif()

if(NOT MSVC)
  target_link_libraries(bwc-cli PRIVATE m)
endif()

if(BWC_WITH_EAS3)
  target_compile_definitions(bwc-cli PRIVATE -DBWC_EAS3)
  MESSAGE(STATUS "EAS3 file format support: ${BIGWHOOP_WITH_EAS3}")
endif()

#----------------------------------------------------------#
# Setup the install directories.                           #
#----------------------------------------------------------#
install(TARGETS bwc-cli DESTINATION ${CMAKE_INSTALL_BINDIR})