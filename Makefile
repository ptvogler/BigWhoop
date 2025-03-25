#*====================================================================================================================*#
#|                                                                                                                    |#
#|                 /$$$$$$$  /$$                  /$$      /$$ /$$                                                    |#
#|                | $$__  $$|__/                 | $$  /$ | $$| $$                                                    |#
#|                | $$  \ $$ /$$  /$$$$$$        | $$ /$$$| $$| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$                 |#
#|                | $$$$$$$ | $$ /$$__  $$       | $$/$$ $$ $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$                |#
#|                | $$__  $$| $$| $$  \ $$       | $$$$_  $$$$| $$  \ $$| $$  \ $$| $$  \ $$| $$  \ $$                |#
#|                | $$  \ $$| $$| $$  | $$       | $$$/ \  $$$| $$  | $$| $$  | $$| $$  | $$| $$  | $$                |#
#|                | $$$$$$$/| $$|  $$$$$$$       | $$/   \  $$| $$  | $$|  $$$$$$/|  $$$$$$/| $$$$$$$/                |#
#|                |_______/ |__/ \____  $$       |__/     \__/|__/  |__/ \______/  \______/ | $$____/                 |#
#|                               /$$  \ $$                                                  | $$                      |#
#|                              |  $$$$$$/                                                  | $$                      |#
#|                               \______/                                                   |__/                      |#
#|                                                                                                                    |#
#|                                                                                                                    |#
#|                                                   Version 0.1.0                                                    |#
#|                                                                                                                    |#
#|  DESCRIPTION:                                                                                                      |#
#|  ------------                                                                                                      |#
#|                Defines a wrapper for the Big Whoop cmake & make tools. This Makefile creates the                   |#
#|								build dir if it does not exist, switches to it and executes cmake and make.           |#
#|                                                                                                                    |#
#|  ----------------------------------------------------------------------------------------------------------------  |#
#|                                                                                                                    |#
#|  Copyright (c) 2021, High Performance Computing Center - University of Stuttgart                                   |#
#|                                                                                                                    |#
#|  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the  |#
#|  following conditions are met:                                                                                     |#
#|                                                                                                                    |#
#|     (1)   Redistributions of source code must retain the above copyright notice, this list of conditions and the   |#
#|           following disclaimer.                                                                                    |#
#|                                                                                                                    |#
#|     (2)   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and    |#
#|           the following disclaimer in the documentation and/or other materials provided with the distribution.     |#
#|                                                                                                                    |#
#|  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED            |#
#|  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A            |#
#|  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY       |#
#|  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,        |#
#|  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER     |#
#|  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR    |#
#|  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH         |#
#|  DAMAGE.                                                                                                           |#
#|                                                                                                                    |#
#*====================================================================================================================*#
#*--------------------------------------------------------*#
# Define special targets for the required build commands.  #
#*--------------------------------------------------------*#
.PHONY: clean
.PHONY: help

.PHONY: static
.PHONY: single

.PHONY: utilities
.PHONY: profiling
.PHONY: omp
.PHONY: eas3
.PHONY: netCDF

.PHONY: build_debug

.PHONY: default
.PHONY: debug
.PHONY: full
.PHONY: release

.PHONY: cmdl
.PHONY: cldebug

#*--------------------------------------------------------*#
# Initialize the compiler flags used to build the library. #
#*--------------------------------------------------------*#
BUILD_TYPE="Release"

BUILD_SHARED="True"

BIGWHOOP_PRECISION="Double"

BUILD_UTILITIES="False"

BIGWHOOP_WITH_PROFILING="False"

BIGWHOOP_WITH_OPENMP="False"

BIGWHOOP_WITH_EAS3="False"

BIGWHOOP_WITH_NETCDF="False"

#*--------------------------------------------------------*#
# Define default target.     				               #
#*--------------------------------------------------------*#
default: 	| build_bwc display

#*--------------------------------------------------------*#
# Define helper target.     		  					   #
#*--------------------------------------------------------*#
help:

	@echo	"Usage: make [options] [argument]                          Compile the BigWhoop library using the specified"
	@echo	"                                                          [options] and [argument]. Only one argument is  "
	@echo	"                                                          accepted per compile run."
	@echo	""
	@echo	"Arguments:"
	@echo   ""
	@echo	"   [Type]                                                 [Description]"
	@echo   ""
	@echo	"   debug                                                  Compiles BigWhoop with all relevant debug flags."
	@echo   ""
	@echo	"   full                                                   Removes all files and folders created during a pre-"
	@echo	"                                                          vious compile run. Compiles BigWhoop (with OpenMP  "
	@echo	"                                                          enabled if applicable). Code optimization is set to"
	@echo	"                                                          the highest level."
	@echo   ""
	@echo	"   release                                                Compiles BigWhoop (with OpenMP enabled if applica-"
	@echo	"                                                          ble). Code optimization is set to the highest level."
	@echo   ""
	@echo	"   cldebug                                                Removes all files and folders created during a pre-"
	@echo	"                                                          vious compile run. Compiles BigWhoop (with OpenMP  "
	@echo	"                                                          enabled if applicable). All relevant debug flags   "
	@echo	"                                                          are set.                                           "
	@echo   ""
	@echo	"   cmdl                                                   Removes all files and folders created during a pre-"
	@echo	"                                                          vious compile run. Compiles BigWhoop (with OpenMP  "
	@echo	"                                                          enabled if applicable). Code optimization is set to"
	@echo	"                                                          the highest level."
	@echo   ""
	@echo	"   clean                                                  Removes all files and folders created during a pre-"
	@echo	"                                                          vious compile run."
	@echo   ""
	@echo	"Options:"
	@echo   ""
	@echo	"   [Type]                                                 [Description]"
	@echo   ""
	@echo	"   static                                                 Builds BigWhoop as a static library."
	@echo   ""
	@echo	"   single                                                 Compiles the BigWhoop library in single precision"
	@echo	"                                                          mode."
	@echo   ""
	@echo	"   utilities                                                   Build the command line tool."
	@echo   ""
	@echo	"   profiling                                              Enable Profiling."
	@echo   ""
	@echo	"   omp                                              	   Enable OpenMP parallelization."
	@echo   ""
	@echo	"   eas3                                                   Adds support for the eas3 file format to the com-"
	@echo	"                                                          mand line tool."
	@echo   ""
	@echo	"   netCDF                                                 Adds support for the eas3 file format to the com-"
	@echo	"                                                          mand line tool."

#*--------------------------------------------------------*#
# Define private targets.                                  #
#*--------------------------------------------------------*#
build_debug:	
	$(eval BUILD_TYPE="Debug")

#*--------------------------------------------------------*#
# Define targets used to instrument library properites.    #
#*--------------------------------------------------------*#
static:
	$(eval BUILD_SHARED="False")

single:
	$(eval BIGWHOOP_PRECISION="Single")

#*--------------------------------------------------------*#
# Define target used to activate command line tool build.  #
#*--------------------------------------------------------*#
utilities:
	$(eval BUILD_UTILITIES="True")

#*--------------------------------------------------------*#
# Define target used to activate profiling.  #
#*--------------------------------------------------------*#
profiling:
	$(eval BIGWHOOP_WITH_PROFILING="True")

#*--------------------------------------------------------*#
# Define target used to activate OpenMP parallelization.   #
#*--------------------------------------------------------*#
omp:
	$(eval BIGWHOOP_WITH_OPENMP="True")

#*--------------------------------------------------------*#
# Define targets used to activate file format support.     #
#*--------------------------------------------------------*#
eas3:
	$(eval BIGWHOOP_WITH_EAS3="True")

netCDF:
	$(eval BIGWHOOP_WITH_NETCDF="True")

#*--------------------------------------------------------*#
# Define the wrappers for the compile command targets.     #
#*--------------------------------------------------------*#
debug:   	| clean build_debug build_bwc display
full:    	| clean build_bwc display
release:	| build_bwc display
cmdl:		| clean utilities profiling eas3 build_bwc display
cldebug:    | clean build_debug utilities profiling eas3 build_bwc display

#*--------------------------------------------------------*#
# Define target used to output compile information.        #
#*--------------------------------------------------------*#
display:
	@echo    "=============================================================="
	@echo	 ""
	@echo 	 "          .:-------------:           .:-------------:         "
	@echo 	 "         .+++++++++++++++=          :+++++++++++++++-         "
	@echo 	 "         :+++.        -++=          -++=                      "
	@echo 	 "         :+++.        -++=          -++=                      "
	@echo 	 "          -++++++++++++++=   -++=   -++=                      "
	@echo 	 "         .=++---------=++=   -++=   -++=                      "
	@echo 	 "         :+++         :++=   -++=   -++=                      "
	@echo 	 "         .+++=--------=+++---=+++---=+++------------:         "
	@echo 	 "          -=++++++++++++++++++++++++++++++++++++++++-         "
	@echo    ""
	@echo    "-----------------   Successfully  Compiled   -----------------"
	@echo    ""
	@echo    "  Build Type:       $(BUILD_TYPE)"
	@echo    "  Shared Libs:      $(BUILD_SHARED)"
	@echo    "  Precision:        $(BIGWHOOP_PRECISION)"
	@echo    "  Utilities:        $(BUILD_UTILITIES)"
	@echo    "  Profiling:        $(BIGWHOOP_WITH_PROFILING)"
	@echo    ""
	@echo    "  Build date:       $(shell date)"
	@echo    "  User:             $(USER)"
	@echo    ""
	@echo    "=============================================================="

#*--------------------------------------------------------*#
# Define the main compile command targets.                 #
#*--------------------------------------------------------*#
build_bwc:
	mkdir -p build && cd build && cmake .. "-DCMAKE_BUILD_TYPE=${BUILD_TYPE}" "-DBUILD_SHARED_LIBS=${BUILD_SHARED}" "-DBIGWHOOP_PRECISION:STRING=${BIGWHOOP_PRECISION}" "-DBUILD_UTILITIES=${BUILD_UTILITIES}" "-DBIGWHOOP_WITH_PROFILING=${BIGWHOOP_WITH_PROFILING}" "-DBIGWHOOP_WITH_OPENMP=${BIGWHOOP_WITH_OPENMP}" "-DBIGWHOOP_WITH_EAS3=${BIGWHOOP_WITH_EAS3}" "-DBIGWHOOP_WITH_NETCDF=${BIGWHOOP_WITH_NETCDF}" && $(MAKE) -j

clean:
	- /bin/rm -rf build/ include/library/public