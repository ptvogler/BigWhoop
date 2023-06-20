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
#|      DESCRIPTION:                                                                                                  |#
#|      ------------                                                                                                  |#
#|                   This file describes python script used to assemble the public header file for the                |#
#|                   BigWhoop compression library.                                                                    |#
#|                                                                                                                    |#
#|      DEVELOPMENT HISTORY:                                                                                          |#
#|      --------------------                                                                                          |#
#|                                                                                                                    |#
#|                   Date        Author             Change Id   Release     Description Of Change                     |#
#|                   ----        ------             ---------   -------     ---------------------                     |#
#|                   02.02.2020  Patrick Vogler     B87D120     V 0.1.0     script created                            |#
#|                                                                                                                    |#
#|                                                                                                                    |#
#|       ------------------------------------------------------------------------------------------------------       |#
#|                                                                                                                    |#
#|       Copyright (c) 2023, High Performance Computing Center - University of Stuttgart                              |#
#|                                                                                                                    |#
#|       Redistribution and use in source and binary forms, with or without modification, are permitted               |#
#|       provided that the following conditions are met:                                                              |#
#|                                                                                                                    |#
#|          (1)   Redistributions of source code must retain the above copyright notice, this list of                 |#
#|                conditions and the following disclaimer.                                                            |#
#|                                                                                                                    |#
#|          (2)   Redistributions in binary form must reproduce the above copyright notice, this list                 |#
#|                of conditions and the following disclaimer in the documentation and/or other materials              |#
#|                provided with the distribution.                                                                     |#
#|                                                                                                                    |#
#|       THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED       |#
#|       WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A       |#
#|       PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR       |#
#|       ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT           |#
#|       LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS          |#
#|       INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR       |#
#|       TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF         |#
#|       ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                   |#
#|                                                                                                                    |#
#\====================================================================================================================/#
from argparse import ArgumentParser
from math     import ceil, floor
from pathlib  import Path

import os
import re
import sys

parser = ArgumentParser(description='Public Header Assembly Script')
parser.add_argument('-OMP', dest='OpenMP', action='store_const',
                    const=True, default=False,
                    help='OpenMP Parallelization')
parser.add_argument('-Single', dest='SinglePrecision', action='store_const',
                    const=True, default=False,
                    help='Compilation using Single Precision Encoder')

args = parser.parse_args()

current_path  = Path().absolute()
source        = current_path.joinpath('include/library/private')

if os.path.isdir('include/library/public') == False:
  os.mkdir('include/library/public')
destination   = current_path.joinpath('include/library/public')



with open(source.joinpath('libbwc.h')) as f:
  regex   = re.compile("(?<=Version )(?:(\d+\.(?:\d+\.)*\d+))")
  Version = ""
  tab     = ""

  for line in f:
    if not Version:
      Version = regex.findall(line)
    if("#include" in line):
      tab = line[0:len(line) - len(line.lstrip(' '))]
    if tab:
      if not Version:
        sys.exit("No Version was specified")
      else:
        break
f.close

deliminator   = tab + "/*" + (116 - len(tab)) * "=" + "*/\n"
ubox          = tab + "/"  + (118 - len(tab)) * "*" + "\\\n"
lbox          = tab + "\\" + (118 - len(tab)) * "*" + "/\n"

public_header = open(destination.joinpath('bwc.h'), 'w+')


public_header.write("/*====================================================================================================================*\\\n"
                    "||                                                                                                                    ||\n"
                    "||                 /$$$$$$$  /$$                  /$$      /$$ /$$                                                    ||\n"
                    "||                | $$__  $$|__/                 | $$  /$ | $$| $$                                                    ||\n"
                    "||                | $$  \ $$ /$$  /$$$$$$        | $$ /$$$| $$| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$                 ||\n"
                    "||                | $$$$$$$ | $$ /$$__  $$       | $$/$$ $$ $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$                ||\n"
                    "||                | $$__  $$| $$| $$  \ $$       | $$$$_  $$$$| $$  \ $$| $$  \ $$| $$  \ $$| $$  \ $$                ||\n"
                    "||                | $$  \ $$| $$| $$  | $$       | $$$/ \  $$$| $$  | $$| $$  | $$| $$  | $$| $$  | $$                ||\n"
                    "||                | $$$$$$$/| $$|  $$$$$$$       | $$/   \  $$| $$  | $$|  $$$$$$/|  $$$$$$/| $$$$$$$/                ||\n"
                    "||                |_______/ |__/ \____  $$       |__/     \__/|__/  |__/ \______/  \______/ | $$____/                 ||\n"
                    "||                               /$$  \ $$                                                  | $$                      ||\n"
                    "||                              |  $$$$$$/                                                  | $$                      ||\n"
                    "||                               \______/                                                   |__/                      ||\n"
                    "||                                                                                                                    ||\n"
                    "||                                                                                                                    ||\n"
                    "||                                                   Version "   + 
                                                                          Version[-1] + "                                                    ||\n"
                    "||                                                                                                                    ||\n"
                    "||       ------------------------------------------------------------------------------------------------------       ||\n"
                    "||                                                                                                                    ||\n"
                    "||       Copyright (c) 2023, High Performance Computing Center - University of Stuttgart                              ||\n"
                    "||                                                                                                                    ||\n"
                    "||       Redistribution and use in source and binary forms, with or without modification, are permitted provided      ||\n"
                    "||       that the following conditions are met:                                                                       ||\n"
                    "||                                                                                                                    ||\n"
                    "||          (1)   Redistributions of source code must retain the above copyright notice, this list of                 ||\n"
                    "||                conditions and the following disclaimer.                                                            ||\n"
                    "||                                                                                                                    ||\n"
                    "||          (2)   Redistributions in binary form must reproduce the above copyright notice, this list                 ||\n"
                    "||                of conditions and the following disclaimer in the documentation and/or other materials              ||\n"
                    "||                provided with the distribution.                                                                     ||\n"
                    "||                                                                                                                    ||\n"
                    "||       THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED       ||\n"
                    "||       WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A       ||\n"
                    "||       PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR       ||\n"
                    "||       ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT           ||\n"
                    "||       LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS          ||\n"
                    "||       INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR       ||\n"
                    "||       TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF         ||\n"
                    "||       ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                   ||\n"
                    "||                                                                                                                    ||\n"
                    "\*====================================================================================================================*/\n"
                    "#ifndef BWC_H\n"
                    "#define BWC_H\n")

lspaces = ceil((116 - 31 - len(tab))/2)
rspaces = floor((116 - 31 - len(tab))/2)

public_header.write(ubox + tab + "||" + lspaces * " " + "_ _  _ ____ _    _  _ ___  ____" + rspaces * " " + "||\n" +
                           tab + "||" + lspaces * " " + "| |\ | |    |    |  | |  \ |___" + rspaces * " " + "||\n" +
                           tab + "||" + lspaces * " " + "| | \| |___ |___ |__| |__/ |___" + rspaces * " " + "||\n" + 
                           tab + "||" + (116 - len(tab)) * " " + "||\n" + lbox)

public_header.write(tab + "#include <stdio.h>\n" + tab + "#include <stdint.h>\n\n")

lspaces = ceil((116 - 62 - len(tab))/2)
rspaces = floor((116 - 62 - len(tab))/2)

public_header.write(ubox + tab + "||" + lspaces * " " + "___  ____ _ _  _ _ ___ _ _  _ ____    ___ _   _ ___  ____ ____" + rspaces * " " + "||\n" +
                           tab + "||" + lspaces * " " + "|__] |__/ | |\/| |  |  | |  | |___     |   \_/  |__] |___ [__ " + rspaces * " " + "||\n" +
                           tab + "||" + lspaces * " " + "|    |  \ | |  | |  |  |  \/  |___     |    |   |    |___ ___]" + rspaces * " " + "||\n" + 
                           tab + "||" + (116 - len(tab)) * " " + "||\n" + lbox)

if(args.SinglePrecision == True):
  file = "prim_types_single.h"
else:
  file = "prim_types_double.h"

print_flag = 0
with open(source.joinpath(file)) as f:
  for line in f:
    if("/*" in line and print_flag == 1):
      break
    if("typedef" in line or print_flag == 1):
      print_flag = 1
      public_header.write(line)
f.close

lspaces = ceil((116 - 29 - len(tab))/2)
rspaces = floor((116 - 29 - len(tab))/2)

public_header.write(ubox + tab + "||" + lspaces * " " + "_  _ ____ ____ ____ ____ ____" + rspaces * " " + "||\n" +
                           tab + "||" + lspaces * " " + "|\/| |__| |    |__/ |  | [__ " + rspaces * " " + "||\n" +
                           tab + "||" + lspaces * " " + "|  | |  | |___ |  \ |__| ___]" + rspaces * " " + "||\n" + 
                           tab + "||" + (116 - len(tab)) * " " + "||\n" + lbox)

with open(source.joinpath('prim_types_double.h')) as f:
  for line in f:
    if("#define" in line):
      if("PREC_BIT" in line):
        public_header.write(line)
      elif("#define MAXIMUM_NO_PASSES" in line):
        public_header.write(line + "\n")
        break
f.close

lspaces = ceil((116 - 42 - len(tab))/2)
rspaces = floor((116 - 42 - len(tab))/2)

public_header.write(ubox + tab + "||" + lspaces * " " + "____ ____ _  _ ____ ___ ____ _  _ ___ ____" + rspaces * " " + "||\n" +
                           tab + "||" + lspaces * " " + "|    |  | |\ | [__   |  |__| |\ |  |  [__ " + rspaces * " " + "||\n" +
                           tab + "||" + lspaces * " " + "|___ |__| | \| ___]  |  |  | | \|  |  ___]" + rspaces * " " + "||\n" + 
                           tab + "||" + (116 - len(tab)) * " " + "||\n" + lbox)

print_flag = -1
with open(source.joinpath('constants.h')) as f:
  for line in f:
    if(";" in line and print_flag == 1):
      print_flag = 0
      public_header.write(line)
    if("typedef" in line or print_flag == 1):
      if(print_flag == 0):
        public_header.write(deliminator)
      print_flag = 1
      public_header.write(line)
f.close

lspaces = ceil((116 - 64 - len(tab))/2)
rspaces = floor((116 - 64 - len(tab))/2)

public_header.write("\n")
public_header.write(ubox + tab + "||" + lspaces * " " + "___  ____ ____ ____ _ _  _ ____ ___     ___ _   _ ___  ____ ____" + rspaces * " " + "||\n" +
                           tab + "||" + lspaces * " " + "|  \ |___ |__/ |__/ | |  | |___ |  \     |   \_/  |__] |___ [__ " + rspaces * " " + "||\n" +
                           tab + "||" + lspaces * " " + "|__/ |___ |  \ |  \ |  \/  |___ |__/     |    |   |    |___ ___]" + rspaces * " " + "||\n" + 
                           tab + "||" + (116 - len(tab)) * " " + "||\n" + lbox)

printFlg    = False
delimFlg    = False
preProcFlg  = False
preProc     = ""
buff        = ""
brktCnt     = 0

for file in ["mq_types.h", "types.h"]:
  with open(source.joinpath(file)) as f:
    for line in f:
      if("typedef" in line):
        while True:
          if("#if" in line):
            if("_OPENMP" in line):
              preProcFlg = True
            while True:
              line = next(f)
              if("endif" in line):
                break
              preProc = preProc + line[len(tab):len(line)]

            if(preProcFlg and args.OpenMP == True):
              buff = buff + preProc

            preProcFlg = False  
            preProc = ""  
          else:
            buff = buff + line

          if("{" in line):
            brktCnt = brktCnt + 1

          if("}" in line):
            brktCnt = brktCnt - 1
            if(brktCnt == 0):
              break
          line = next(f)

        if("bwc_" in line):
          if(printFlg == True):
            buff = deliminator + buff
          else:
            printFlg = True
          public_header.write(buff)

        buff = ""
  f.close

lspaces = ceil((116 - 70 - len(tab))/2)
rspaces = floor((116 - 70 - len(tab))/2)

public_header.write("\n" + ubox + tab + "||" + lspaces * " " + "___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____" + rspaces * " " + "||\n" +
                                  tab + "||" + lspaces * " " + "|__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__ " + rspaces * " " + "||\n" +
                                  tab + "||" + lspaces * " " + "|    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]" + rspaces * " " + "||\n" + 
                                  tab + "||" + (116 - len(tab)) * " " + "||\n" + lbox)

files = os.listdir("include/library/private")
printFlg = False
ltab     = 0
tmp      = ""
buff     = ""

for file in files:
  with open(source.joinpath(file)) as f:
    for line in f:

      if("#ifdef" in line or "#if defined" in line):
        if("_OPENMP" in line and args.OpenMP == True):
          ltab = len(tab)
          line = next(f)
        else:
          while ("#endif" not in line):
            line = next(f)
      if("#endif" in line):
        ltab = 0

      if("(" in line):
        tmp = line[0:line.index('(')]
        tmp = tmp[tmp.rfind(' '):-1]
      else:
        tmp = ""
      if("bwc_" in tmp and "!" not in line):
        if("/*" in buff or "*/" in buff):
          buff = ""
        else:
          buff = buff[ltab:len(buff)]

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
public_header.write("#endif")
public_header.close