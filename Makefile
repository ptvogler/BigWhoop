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

.PHONY: tool
.PHONY: eas3
.PHONY: netCDF

.PHONY: --build_debug

.PHONY: default
.PHONY: debug
.PHONY: full

#*--------------------------------------------------------*#
# Initialize the compiler flags used to build the library. #
#*--------------------------------------------------------*#
BUILD_TYPE="Release"

LINK_TYPE="Dynamic"

BUILD_PREC="Double"

BUILD_TOOL="False"

BUILD_EAS3="False"

BUILD_NETCDF="False"

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
	@echo	"   debug                                                  Compiles BigWhoop and the command line tool with "
	@echo	"                                                          full file support. All relevant debug flags are set."
	@echo   ""
	@echo	"   full                                                   Compiles BigWhoop (with OpenMP enabled if applica-"
	@echo	"                                                          ble) and the command line tool with full file sup-"
	@echo	"                                                          port. Code optimization is set to the highest level."
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
	@echo	"   tool                                                   Build the command line tool."
	@echo   ""
	@echo	"   eas3                                                   Adds support for the eas3 file format to the com-"
	@echo	"                                                          mand line tool."
	@echo   ""
	@echo	"   netCDF                                                 Adds support for the eas3 file format to the com-"
	@echo	"                                                          mand line tool."

#*--------------------------------------------------------*#
# Define private targets.                                  #
#*--------------------------------------------------------*#
--build_debug:	
	$(eval BUILD_TYPE="Debug")

#*--------------------------------------------------------*#
# Define targets used to instrument library properites.    #
#*--------------------------------------------------------*#
static:
	$(eval LINK_TYPE="Static")

single:
	$(eval BUILD_PREC="Single")

#*--------------------------------------------------------*#
# Define target used to activate command line tool build.  #
#*--------------------------------------------------------*#
tool:
	$(eval BUILD_TOOL="True")


#*--------------------------------------------------------*#
# Define targets used to activate file format support.     #
#*--------------------------------------------------------*#
eas3:
	$(eval BUILD_EAS3="True")

netCDF:
	$(eval BUILD_NETCDF="True")

#*--------------------------------------------------------*#
# Define the wrappers for the compile command targets.     #
#*--------------------------------------------------------*#
debug:   	| clean --build_debug tool eas3 build_bwc display
release:	| build_bwc display
full:    	| clean tool eas3 build_bwc display

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
	@echo    "  Link Type:        $(LINK_TYPE)"
	@echo    "  Precision:        $(BUILD_PREC)"
	@echo    "  Utilities:        $(BUILD_TOOL)"
	@echo    ""
	@echo    "  Build date:       $(shell date)"
	@echo    "  User:             $(USER)"
	@echo    ""
	@echo    "=============================================================="

#*--------------------------------------------------------*#
# Define the main compile command targets.                 #
#*--------------------------------------------------------*#
build_bwc:
	mkdir -p build && cd build && cmake .. "-DCMAKE_BUILD_TYPE=${BUILD_TYPE}" "-DLINK:STRING=${LINK_TYPE}" "-DPREC:STRING=${BUILD_PREC}" "-DTOOL=${BUILD_TOOL}" "-DBUILD_EAS3=${BUILD_EAS3}" "-DBUILD_NETCDF=${BUILD_NETCDF}" && $(MAKE) -j

clean:
	- /bin/rm -rf build/ bin/ lib/ lib64/ include/library/public