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
#|        Defines the cmake script for BigWhoops HDF5 plugin.                                     |#
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
# Define the HDF5 plugin.                                  #
#----------------------------------------------------------#
add_library(h5z_bwc SHARED H5Zbwc.c)

#----------------------------------------------------------#
# Setup up the include directory for the HDF5 plugin.      #
#----------------------------------------------------------#
target_include_directories(h5z_bwc
  PRIVATE
    ${HDF5_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include/interfaces/HDF5    
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/interfaces/HDF5>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/bwc>)

#----------------------------------------------------------#
# Link the plugin to the BigWhoop and HDF5 library.        #
#----------------------------------------------------------#
if(TARGET bwc_objects)
  target_link_libraries(h5z_bwc PRIVATE bwc_objects HDF5::HDF5)
  message(STATUS "Building HDF5 plugin inside bwc source tree (using bwc_objects).")

elseif(TARGET bwc::bwclib_static)
  target_link_libraries(h5z_bwc PRIVATE bwc::bwclib_static HDF5::HDF5)
  message(STATUS "Building HDF5 plugin against installed static bwc library.")

else()
  message(FATAL_ERROR "Cannot build HDF5 plugin: "
            "Target 'bwc_objects' (internal) or 'bwc::bwclib_static' (installed) not found. "
            "The plugin requires a static link to the bwc library.")
endif()

#----------------------------------------------------------#
# Set the properties of the HDF5 plugin.                   #
#----------------------------------------------------------#
set_target_properties(h5z_bwc PROPERTIES 
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  OUTPUT_NAME "h5z_bwc"
  PUBLIC_HEADER "${CMAKE_SOURCE_DIR}/include/interfaces/HDF5/H5Zbwc.h")

#----------------------------------------------------------#
# Setup the install directories and target exporting for   #
# config-file packaging.                                   #
#----------------------------------------------------------#
install(TARGETS h5z_bwc
  EXPORT bwc-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/plugin
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/bwc)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  message(WARNING 
    "HDF5 plugin installed to ${CMAKE_INSTALL_LIBDIR}/plugin.\n"
    "Set HDF5_PLUGIN_PATH to this directory to use it.")
endif()