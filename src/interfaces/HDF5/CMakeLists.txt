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
# Define the library target                                #
#----------------------------------------------------------#
add_library(h5z_bwc SHARED H5Zbwc.c)

#----------------------------------------------------------#
# Setup up the include directory for the BigWhoop library. #
#----------------------------------------------------------#
target_include_directories(h5z_bwc PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/library/public>
                                   PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                                           ${HDF5_INCLUDE_DIRS})

#----------------------------------------------------------#
# Link the BigWhoop library to the math.h library.         #
#----------------------------------------------------------#
target_link_libraries(h5z_bwc PRIVATE bwclib ${HDF5_LIBRARIES})

#----------------------------------------------------------#
# Set the Version and SOVersion and define the public API  # 
# for the HDF5 plugin.                                     #
#----------------------------------------------------------#
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/H5Zbwc.c _H5Zbwc_c_contents)
string(REGEX REPLACE ".*#define[ \t]+H5Z_BWC_VERSION_MAJOR[ \t]+([0-9]+).*"
     "\\1" H5Z_BWC_VERSION_MAJOR ${_H5Zbwc_c_contents})
string(REGEX REPLACE ".*#define[ \t]+H5Z_BWC_VERSION_MINOR[ \t]+([0-9]+).*"
     "\\1" H5Z_BWC_VERSION_MINOR ${_H5Zbwc_c_contents})
string(REGEX REPLACE ".*#define[ \t]+H5Z_BWC_VERSION_PATCH[ \t]+([0-9]+).*"
     "\\1" H5Z_BWC_VERSION_PATCH ${_H5Zbwc_c_contents})
set(H5Z_BWC_VERSION "${H5Z_BWC_VERSION_MAJOR}.${H5Z_BWC_VERSION_MINOR}.${H5Z_BWC_VERSION_PATCH}")

set_target_properties(h5z_bwc PROPERTIES VERSION "${H5Z_BWC_VERSION}"
                                       SOVERSION "${H5Z_BWC_VERSION_MAJOR}"
                                   PUBLIC_HEADER "${CMAKE_SOURCE_DIR}/include/interfaces/HDF5/H5Z_bwc.h"
                                   OUTPUT_NAME   h5z_bwc)

#----------------------------------------------------------#
# Setup the install directories and target exporting for   #
# config-file packaging.                                   #
#----------------------------------------------------------#
install(TARGETS                   h5z_bwc
        LIBRARY DESTINATION       ${CMAKE_INSTALL_LIBDIR}/plugin
        ARCHIVE DESTINATION       ${CMAKE_INSTALL_LIBDIR}/plugin
        RUNTIME DESTINATION       ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#----------------------------------------------------------#
# Add the HDF5 test utility to the current project.        #
#----------------------------------------------------------#
add_executable(test_hdf5  test_hdf5.c)

#----------------------------------------------------------#
# Define the output name for the utility binaries.         #
#----------------------------------------------------------#
set_property(TARGET test_hdf5 PROPERTY OUTPUT_NAME test_hdf5)

#----------------------------------------------------------#
# Setup up the include directory for the bwc utilities.    #
#----------------------------------------------------------#
target_include_directories(test_hdf5 PRIVATE ${CMAKE_SOURCE_DIR}/include/interfaces/HDF5)

#----------------------------------------------------------#
# Setup the install directories.                           #
#----------------------------------------------------------#
#install(TARGETS test_hdf5 RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

#----------------------------------------------------------#
# Link the bwc utility to the bwc library.                 #
#----------------------------------------------------------#
target_link_libraries(test_hdf5 PRIVATE bwclib m ${HDF5_LIBRARIES})

#----------------------------------------------------------#
# Add the utility to the test regiment.                    #
#----------------------------------------------------------#
add_test(NAME test_hdf5 COMMAND test_hdf5)