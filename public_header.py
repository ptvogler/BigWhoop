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
#|        This file describes python script used to assemble the public header file for           |#
#|        the BigWhoop compression library.                                                       |#
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
#**************************************************************************************************#
#|                                   _ _  _ ___  ____ ____ ___                                    |#
#|                                   | |\/| |__] |  | |__/  |                                     |#
#|                                   | |  | |    |__| |  \  |                                     |#
#|                                                                                                |#
#**************************************************************************************************#
from argparse import ArgumentParser
from math     import ceil, floor
from pathlib  import Path

import os
import re
import sys

#-------------------------#
# DEFINE CONSTANTS:       #
#-------------------------#
tab           = "  "
text_width    = 100
deliminator   = tab + "/*" + (text_width - 4 - len(tab)) * "=" + "*/\n"
ubox          = tab + "/"  + (text_width - 2 - len(tab)) * "*" + "\\\n"
lbox          = tab + "\\" + (text_width - 2 - len(tab)) * "*" + "/\n"
sbox          = "||"

#----------------------------------------------------------#
# Setup the argument parser for the public header script   #
# and parse for the user defined OpenMP and Precision arg. #
#----------------------------------------------------------#
parser = ArgumentParser(description='Public Header Assembly Script')
parser.add_argument('-OMP', dest='OpenMP', action='store_const',
                    const=True, default=False,
                    help='OpenMP Parallelization')
parser.add_argument('-Single', dest='SinglePrecision', action='store_const',
                    const=True, default=False,
                    help='Compilation using Single Precision Encoder')

args = parser.parse_args()


#----------------------------------------------------------#
# Setup the paths to the source and destination.           #
#----------------------------------------------------------#
current_path  = Path().absolute()
source        = current_path.joinpath('include/library/private')
destination   = current_path.joinpath('include/library/public')

if os.path.isdir(destination) == False:
  os.mkdir(destination)

include_files = ['macros.h', 'constants.h', 'dwt.h', 'tagtree.h', 'mq_types.h', 'mq.h',
                'bitstream.h', 'codestream.h', 'tier1.h', 'tier2.h', 'types.h', 'libbwc.h']
exclude_files = ["prim_types_double.h", "prim_types_single.h"]
all_files = [f for f in os.listdir(source) if os.path.isfile(os.path.join(source, f))]
missing_files = [f for f in all_files if f not in include_files and f not in exclude_files]
if missing_files:
  raise Warning("Consider updating public_header.py. Missing header file(s)"
               f"\n{missing_files}\n"
                "should be added to include_files or exclude_files lists.")
print(include_files)

#----------------------------------------------------------#
# Create the I/O stream and write the bwc file header.     #
#----------------------------------------------------------#
public_header = open(destination.joinpath('bwc.h'), 'w+')
public_header.write("/*================================================================================================*\\\n"
                    "||                                                                                                ||\n"
                    "||       /$$$$$$$  /$$                  /$$      /$$ /$$                                          ||\n"
                    "||      | $$__  $$|__/                 | $$  /$ | $$| $$                                          ||\n"
                    "||      | $$  \ $$ /$$  /$$$$$$        | $$ /$$$| $$| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$       ||\n"
                    "||      | $$$$$$$ | $$ /$$__  $$       | $$/$$ $$ $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$      ||\n"
                    "||      | $$__  $$| $$| $$  \ $$       | $$$$_  $$$$| $$  \ $$| $$  \ $$| $$  \ $$| $$  \ $$      ||\n"
                    "||      | $$  \ $$| $$| $$  | $$       | $$$/ \  $$$| $$  | $$| $$  | $$| $$  | $$| $$  | $$      ||\n"
                    "||      | $$$$$$$/| $$|  $$$$$$$       | $$/   \  $$| $$  | $$|  $$$$$$/|  $$$$$$/| $$$$$$$/      ||\n"
                    "||      |_______/ |__/ \____  $$       |__/     \__/|__/  |__/ \______/  \______/ | $$____/       ||\n"
                    "||                     /$$  \ $$                                                  | $$            ||\n"
                    "||                    |  $$$$$$/                                                  | $$            ||\n"
                    "||                     \______/                                                   |__/            ||\n"
                    "||                                                                                                ||\n"
                    "||  --------------------------------------------------------------------------------------------  ||\n"
                    "||  Copyright (c) 2023, High Performance Computing Center - University of Stuttgart               ||\n"
                    "||                                                                                                ||\n"
                    "||  Redistribution and use in source and binary forms, with or without modification, are          ||\n"
                    "||  permitted provided that the following conditions are met:                                     ||\n"
                    "||                                                                                                ||\n"
                    "||     (1)   Redistributions of source code must retain the above copyright notice, this list of  ||\n"
                    "||           conditions and the following disclaimer.                                             ||\n"
                    "||                                                                                                ||\n"
                    "||     (2)   Redistributions in binary form must reproduce the above copyright notice, this list  ||\n"
                    "||           of conditions and the following disclaimer in the documentation and/or other         ||\n"
                    "||           materials provided with the distribution.                                            ||\n"
                    "||                                                                                                ||\n"
                    "||  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS   ||\n"
                    "||  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF               ||\n"
                    "||  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE    ||\n"
                    "||  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,     ||\n"
                    "||  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF            ||\n"
                    "||  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)        ||\n"
                    "||  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR      ||\n"
                    "||  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  ||\n"
                    "||  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                            ||\n"
                    "||                                                                                                ||\n"
                    "\*================================================================================================*/\n"
                    "#ifndef BWC_H\n"
                    "#define BWC_H\n")

#----------------------------------------------------------#
# Generate the necessary includes for the bwc.h file.      #
#----------------------------------------------------------#
title_length = 31

lspaces = ceil((text_width - 2*len(sbox) - title_length - len(tab))/2)
rspaces = floor((text_width - 2*len(sbox) - title_length - len(tab))/2)

public_header.write(ubox + tab + sbox + lspaces * " " + "_ _  _ ____ _    _  _ ___  ____" + rspaces * " " + sbox + "\n" +
                           tab + sbox + lspaces * " " + "| |\ | |    |    |  | |  \ |___" + rspaces * " " + sbox + "\n" +
                           tab + sbox + lspaces * " " + "| | \| |___ |___ |__| |__/ |___" + rspaces * " " + sbox + "\n" + 
                           tab + sbox + (text_width - 2*len(sbox) - len(tab)) * " " + "||\n" + lbox)

public_header.write(tab + "#include <stdio.h>\n" + tab + "#include <stdint.h>\n\n")

#----------------------------------------------------------#
# Ingest primitive type definitions from the corresponding #
# header files according to the user specified precision.  #
#----------------------------------------------------------#
title_length = 62

lspaces = ceil((text_width - 2*len(sbox) - title_length - len(tab))/2)
rspaces = floor((text_width - 2*len(sbox) - title_length - len(tab))/2)

public_header.write(ubox + tab + sbox + lspaces * " " + "___  ____ _ _  _ _ ___ _ _  _ ____    ___ _   _ ___  ____ ____" + rspaces * " " + "||\n" +
                           tab + sbox + lspaces * " " + "|__] |__/ | |\/| |  |  | |  | |___     |   \_/  |__] |___ [__ " + rspaces * " " + "||\n" +
                           tab + sbox + lspaces * " " + "|    |  \ | |  | |  |  |  \/  |___     |    |   |    |___ ___]" + rspaces * " " + "||\n" + 
                           tab + sbox + (text_width - 2*len(sbox) - len(tab)) * " " + "||\n" + lbox)

if(args.SinglePrecision == True):
  file = "prim_types_single.h"
else:
  file = "prim_types_double.h"

print_flag = False
with open(source.joinpath(file)) as f:
  for line in f:
    if("/*" in line and print_flag == 1):
      break
    if("typedef" in line or print_flag == 1):
      print_flag = True
      public_header.write(line)
f.close

#----------------------------------------------------------#
# Ingest Macros with 'BWC_' prefix from all header files.  #
#----------------------------------------------------------#
title_length = 29

lspaces = ceil((text_width - 2*len(sbox) - title_length - len(tab))/2)
rspaces = floor((text_width - 2*len(sbox) - title_length - len(tab))/2)

public_header.write(ubox + tab + sbox + lspaces * " " + "_  _ ____ ____ ____ ____ ____" + rspaces * " " + "||\n" +
                           tab + sbox + lspaces * " " + "|\/| |__| |    |__/ |  | [__ " + rspaces * " " + "||\n" +
                           tab + sbox + lspaces * " " + "|  | |  | |___ |  \ |__| ___]" + rspaces * " " + "||\n" + 
                           tab + sbox + (text_width - 2*len(sbox) - len(tab)) * " " + "||\n" + lbox)

with open(source.joinpath(file)) as f:
  for line in f:
    if("#define" in line):
      if("MAXIMUM_NO_PASSES" in line or
         "PREC_BIT" in line):
        if("//" in line):
          public_header.write((line.split('/', 1)[0]).rstrip() + '\n')
        else:
          public_header.write(line)
f.close

printFlg    = False
buff        = ""
brktCnt     = 0

for file in include_files:
  with open(source.joinpath(file)) as f:
    for line in f:
      if("BWC_" in line):
        if("ifndef" in line):
          next(f)
        elif("#define" in line and line[len(line) - len(line.lstrip())] != "/"):
          while True:
            if("//" in line):
              buff = buff + (line.split('/', 1)[0]).rstrip() + '\n'
            else:
              buff = buff + line
            if("\\" not in line):
              public_header.write(buff+"\n")
              buff = ""
              break
            line = next(f)
  f.close
public_header.write("\n")

#----------------------------------------------------------#
# Ingest enums with 'bwc_' prefix from all header files    #
# excluding prim_types_****.h.                             #
#----------------------------------------------------------#
title_length = 42

lspaces = ceil((text_width - 2*len(sbox) - title_length - len(tab))/2)
rspaces = floor((text_width - 2*len(sbox) - title_length - len(tab))/2)

public_header.write(ubox + tab + sbox + lspaces * " " + "____ ____ _  _ ____ ___ ____ _  _ ___ ____" + rspaces * " " + "||\n" +
                           tab + sbox + lspaces * " " + "|    |  | |\ | [__   |  |__| |\ |  |  [__ " + rspaces * " " + "||\n" +
                           tab + sbox + lspaces * " " + "|___ |__| | \| ___]  |  |  | | \|  |  ___]" + rspaces * " " + "||\n" + 
                           tab + sbox + (text_width - 2*len(sbox) - len(tab)) * " " + "||\n" + lbox)

delimFlg    = False
buff        = ""

for file in include_files:
  with open(source.joinpath(file)) as f:
    for line in f:
      if("typedef enum" in line):
        while True:
          buff = buff + (line.split('/', 1)[0]).rstrip() + '\n'
          if("}" in line and ";" in line):
            if("bwc_" in line):
              if(delimFlg == True):
                public_header.write(deliminator)
              public_header.write(buff)
              delimFlg = True
            buff = ""
            break
          line = next(f)
  f.close

#----------------------------------------------------------#
# Ingest derived types with 'bwc_' prefix from all header  #
# files excluding prim_types_****.h.                       #
#----------------------------------------------------------#
title_length = 64

lspaces = ceil((text_width - 2*len(sbox) - title_length - len(tab))/2)
rspaces = floor((text_width - 2*len(sbox) - title_length - len(tab))/2)

public_header.write("\n")
public_header.write(ubox + tab + sbox + lspaces * " " + "___  ____ ____ ____ _ _  _ ____ ___     ___ _   _ ___  ____ ____" + rspaces * " " + "||\n" +
                           tab + sbox + lspaces * " " + "|  \ |___ |__/ |__/ | |  | |___ |  \     |   \_/  |__] |___ [__ " + rspaces * " " + "||\n" +
                           tab + sbox + lspaces * " " + "|__/ |___ |  \ |  \ |  \/  |___ |__/     |    |   |    |___ ___]" + rspaces * " " + "||\n" + 
                           tab + sbox + (text_width - 2*len(sbox) - len(tab)) * " " + "||\n" + lbox)

printFlg    = False
delimFlg    = False
preProcFlg  = False
buff        = ""
brktCnt     = 0

for file in include_files:
  with open(source.joinpath(file)) as f:
    for line in f:
      if("typedef struct" in line or
         "typedef union"  in line):
        if("bwc_" in line):
          printFlg = True
        while True:
          buff = buff + (line.split('/', 1)[0]).rstrip() + '\n'
          if("{" in line):
            brktCnt = brktCnt + 1
          if("}" in line):
            brktCnt = brktCnt - 1
            if(brktCnt == 0):
              if("bwc_" in line):
                printFlg = True
              break
          line = next(f)
        if (printFlg == True):
          if(delimFlg == True):
            public_header.write(deliminator)
          public_header.write(buff)
          delimFlg = True
        printFlg = False
        buff = ""

  f.close

#----------------------------------------------------------#
# Ingest public functions with 'bwc_' prefix from all      #
# header files excluding prim_types_****.h.                #
#----------------------------------------------------------#
title_length = 70

lspaces = ceil((text_width - 2*len(sbox) - title_length - len(tab))/2)
rspaces = floor((text_width - 2*len(sbox) - title_length - len(tab))/2)

public_header.write("\n" + ubox + tab + sbox + lspaces * " " + "___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____" + rspaces * " " + "||\n" +
                                  tab + sbox + lspaces * " " + "|__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__ " + rspaces * " " + "||\n" +
                                  tab + sbox + lspaces * " " + "|    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]" + rspaces * " " + "||\n" + 
                                  tab + sbox + (text_width - 2*len(sbox) - len(tab)) * " " + "||\n" + lbox)

public_header.write(tab + "#ifdef __cplusplus\n" +
                    tab + tab + "extern \"C\" {\n" +
                    tab + "#endif\n\n")

files = os.listdir("include/library/private")
printFlg = False
ltab     = 0
tmp      = ""
buff     = ""

for file in files:
  with open(source.joinpath(file)) as f:
    for line in f:
      if("#if defined" in line):
        line = next(f)
      if("(" in line):
        tmp = line[0:line.index('(')].strip()
        tmp = tmp[tmp.rfind(' '):-1]
      else:
        tmp = ""
      if("bwc_" in tmp and "!" not in line and "#if" not in line):
        if("/*" in buff or "*/" in buff or "//" in buff):
          buff = ""

        if(print_flag == True):
          buff = deliminator + buff
        else:
          print_flag = True

        while True:
          buff = buff + line[ltab:len(line)]
          if(");" in line):
            break
          line = next(f)
               
        public_header.write(buff)
      buff = line
  f.close

public_header.write("\n" + tab + "#ifdef __cplusplus\n" +
                                  tab + tab + "}\n" +
                                  tab + "#endif\n")

public_header.write("#endif")
public_header.close